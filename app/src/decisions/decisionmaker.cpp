#include "src/decisions/decisionmaker.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr float HUNDRED_PERCENT = 100.0f;



DecisionMaker::DecisionMaker(
    IInstrumentsStorage*           instrumentsStorage,
    IUserStorage*                  userStorage,
    const QList<IActionDecision*>& buyDecisions,
    const QList<IActionDecision*>& sellDecisions
) :
    IDecisionMaker(),
    mInstrumentsStorage(instrumentsStorage),
    mUserStorage(userStorage),
    mBuyDecisions(buyDecisions),
    mSellDecisions(sellDecisions),
    mStocksMap()
{
    qDebug() << "Create DecisionMaker";
}

DecisionMaker::~DecisionMaker()
{
    qDebug() << "Destroy DecisionMaker";
}

InstrumentsForTrading DecisionMaker::makeDecision(
    QThread*             parentThread,
    qint64               timestamp,
    IConfig*             config,
    const Portfolio&     portfolio,
    const QList<Stock*>& stocks,
    bool                 autoPilot,
    int                  keepMoney,
    bool                 dateRange,
    bool                 useParallel
)
{
    InstrumentsForTrading res;

    if (config->isUseSchedule())
    {
        const QDateTime dateTime  = QDateTime::fromMSecsSinceEpoch(timestamp); // TODO: Moscow time?
        const int       dayOfWeek = dateTime.date().dayOfWeek();

        if (dayOfWeek == Qt::Saturday || dayOfWeek == Qt::Sunday)
        {
            return res;
        }

        const QTime time        = dateTime.time();
        const int   startHour   = config->getScheduleStartHour();
        const int   startMinute = config->getScheduleStartMinute();
        const int   endHour     = config->getScheduleEndHour();
        const int   endMinute   = config->getScheduleEndMinute();
        const QTime startTime   = QTime(startHour, startMinute);
        const QTime endTime     = QTime(endHour, endMinute);

        if (time < startTime || time > endTime)
        {
            return res;
        }
    }

    IDecisionMakerConfig*    decisionConfig = chooseDecisionConfig(config, autoPilot);
    QList<StockWithAvgPrice> stocksWithAvgPrice;

    updateStocksMap(stocks);
    getStocksWithAvgPrice(portfolio, stocks, stocksWithAvgPrice);

    makeDecisions(
        parentThread, config, decisionConfig, timestamp, portfolio, stocksWithAvgPrice, keepMoney, dateRange, useParallel, res
    );

    return res;
}

IDecisionMakerConfig* DecisionMaker::chooseDecisionConfig(IConfig* config, bool autoPilot)
{
    if (config->isSimulatorConfigCommon())
    {
        return config->getSimulatorConfig();
    }

    if (config->isAutoPilotConfigCommon() || autoPilot)
    {
        return config->getAutoPilotConfig();
    }

    return config->getSimulatorConfig();
}

void DecisionMaker::updateStocksMap(const QList<Stock*>& stocks)
{
    if (mStocksMap.size() != stocks.size())
    {
        for (Stock* stock : stocks)
        {
            mStocksMap[stock->meta.instrumentId] = stock;
        }
    }
}

void DecisionMaker::getStocksWithAvgPrice(
    const Portfolio& portfolio, const QList<Stock*>& stocks, QList<StockWithAvgPrice>& stocksWithAvgPrice
)
{
    mUserStorage->readLock();
    const bool qualifiedUser = mUserStorage->isQualified();
    mUserStorage->readUnlock();

    QMap<QString, float> existingStocks; // Instrument UID => Average price

    for (const PortfolioCategoryItem& category : portfolio.positions)
    {
        for (const PortfolioItem& item : category.items)
        {
            if (mStocksMap.contains(item.instrumentId))
            {
                existingStocks[item.instrumentId] = item.avgPriceWavg;
            }
        }
    }

    for (Stock* stock : stocks)
    {
        stock->readLock();

        const float avgPrice = existingStocks.value(stock->meta.instrumentId, -1);

        if (avgPrice < 0)
        {
            if (qualifiedUser || !stock->meta.forQualInvestorFlag)
            {
                stocksWithAvgPrice.append(StockWithAvgPrice(stock, avgPrice));
            }
        }
        else
        {
            stocksWithAvgPrice.append(StockWithAvgPrice(stock, avgPrice));
        }

        stock->readUnlock();
    }
}

struct MakeDecisionsInfo
{
    explicit MakeDecisionsInfo(
        IDecisionMakerConfig*    _decisionConfig,
        qint64                   _timestamp,
        bool                     _dateRange,
        float                    _money,
        float                    _commission,
        QList<IActionDecision*>* _buyDecisions,
        QList<IActionDecision*>* _sellDecisions,
        int                      _threadCount
    ) :
        decisionConfig(_decisionConfig),
        timestamp(_timestamp),
        dateRange(_dateRange),
        money(_money),
        commission(_commission),
        buyDecisions(_buyDecisions),
        sellDecisions(_sellDecisions)
    {
        buyResults.resize(_threadCount);
        sellResults.resize(_threadCount);
    }

    IDecisionMakerConfig*        decisionConfig;
    qint64                       timestamp;
    bool                         dateRange;
    float                        money;
    float                        commission;
    QList<IActionDecision*>*     buyDecisions;
    QList<IActionDecision*>*     sellDecisions;
    QList<InstrumentsForTrading> buyResults;
    QList<InstrumentsForTrading> sellResults;
};

static void makeDecisionsForParallel(
    QThread* parentThread, int threadId, QList<StockWithAvgPrice>& stocks, int start, int end, void* additionalArgs
)
{
    MakeDecisionsInfo* makeDecisionsInfo = reinterpret_cast<MakeDecisionsInfo*>(additionalArgs);

    IDecisionMakerConfig*  decisionConfig     = makeDecisionsInfo->decisionConfig;
    const qint64           timestamp          = makeDecisionsInfo->timestamp;
    const bool             dateRange          = makeDecisionsInfo->dateRange;
    const float            money              = makeDecisionsInfo->money;
    const float            commission         = makeDecisionsInfo->commission;
    IActionDecision**      buyDecisionsArray  = makeDecisionsInfo->buyDecisions->data();
    const int              buyDecisionsSize   = makeDecisionsInfo->buyDecisions->size();
    IActionDecision**      sellDecisionsArray = makeDecisionsInfo->sellDecisions->data();
    const int              sellDecisionsSize  = makeDecisionsInfo->sellDecisions->size();
    InstrumentsForTrading* buyResultsArray    = makeDecisionsInfo->buyResults.data();
    InstrumentsForTrading* sellResultsArray   = makeDecisionsInfo->sellResults.data();

    StockWithAvgPrice* stocksArray = stocks.data();

    int   dataIndex = 0;
    float price     = 0;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock*      stock    = stocksArray[i].stock;
        const float avgPrice = stocksArray[i].avgPrice;

        stock->readLock();

        if (dateRange)
        {
            dataIndex = std::distance(
                stock->data.constBegin(),
                std::lower_bound(
                    stock->data.constBegin(), stock->data.constEnd(), timestamp, [](const StockData& stockData, qint64 value) {
                        return stockData.timestamp < value;
                    }
                )
            );

            if (dataIndex >= stock->data.size() || stock->data.at(dataIndex).timestamp != timestamp)
            {
                stock->readUnlock();

                continue;
            }

            price = stock->data.at(dataIndex).price;
        }
        else
        {
            dataIndex = -1;
            price     = stock->lastPrice();
        }

        QString cause;

        if (avgPrice < 0)
        {
            if (money >= price)
            {
                for (int j = 0; j < buyDecisionsSize && cause == "" && !parentThread->isInterruptionRequested(); ++j)
                {
                    cause = buyDecisionsArray[j]->makeDecision(
                        parentThread, decisionConfig, stock, dateRange, dataIndex, price, avgPrice, commission
                    );
                }

                if (cause != "")
                {
                    TradingInfo tradingInfo;

                    tradingInfo.price        = price;
                    tradingInfo.expectedCost = stock->meta.turnover;
                    tradingInfo.cause        = cause;

                    buyResultsArray[threadId][stock->meta.instrumentId] = tradingInfo;
                }
            }
        }
        else
        {
            for (int j = 0; j < sellDecisionsSize && cause == "" && !parentThread->isInterruptionRequested(); ++j)
            {
                cause = sellDecisionsArray[j]->makeDecision(
                    parentThread, decisionConfig, stock, dateRange, dataIndex, price, avgPrice, commission
                );
            }

            if (cause != "")
            {
                TradingInfo tradingInfo;

                tradingInfo.price        = price;
                tradingInfo.expectedCost = 0.0;
                tradingInfo.cause        = cause;

                sellResultsArray[threadId][stock->meta.instrumentId] = tradingInfo;
            }
        }

        stock->readUnlock();
    }
}

void DecisionMaker::makeDecisions(
    QThread*                  parentThread,
    IConfig*                  config,
    IDecisionMakerConfig*     decisionConfig,
    qint64                    timestamp,
    const Portfolio&          portfolio,
    QList<StockWithAvgPrice>& stocksWithAvgPrice,
    int                       keepMoney,
    bool                      dateRange,
    bool                      useParallel,
    InstrumentsForTrading&    res
)
{
    double totalCost = 0.0;
    double money     = 0.0;

    calculateTotalCostAndMoney(portfolio, totalCost, money);
    money -= keepMoney;

    mUserStorage->readLock();
    float commission = mUserStorage->getCommission();
    mUserStorage->readUnlock();

    MakeDecisionsInfo makeDecisionsInfo(
        decisionConfig, timestamp, dateRange, money, commission, &mBuyDecisions, &mSellDecisions, useParallel ? getCpuCount() : 1
    );

    if (useParallel)
    {
        processInParallel(parentThread, stocksWithAvgPrice, makeDecisionsForParallel, &makeDecisionsInfo);
    }
    else
    {
        makeDecisionsForParallel(parentThread, 0, stocksWithAvgPrice, 0, stocksWithAvgPrice.size(), &makeDecisionsInfo);
    }

    commission /= HUNDRED_PERCENT;

    for (const InstrumentsForTrading& result : std::as_const(makeDecisionsInfo.sellResults))
    {
        res.insert(result);
    }

    for (const InstrumentsForTrading& result : std::as_const(makeDecisionsInfo.buyResults))
    {
        for (auto it = result.constBegin(); it != result.constEnd(); ++it)
        {
            const QString& instrumentId = it.key();
            TradingInfo    tradingInfo  = it.value();

            mInstrumentsStorage->readLock();
            Instrument instrument = mInstrumentsStorage->getInstruments().value(instrumentId);
            mInstrumentsStorage->readUnlock();

            instrument.resetIfNotFound(instrumentId);

            const double lotPrice               = instrument.lot * tradingInfo.price;
            const double lotPriceWithCommission = lotPrice * (1 + commission);

            qint64 amountOfLots = 0;

            if (config->isLimitStockPurchase())
            {
                double cost = 0.0;

                if (config->isLimitByTurnover())
                {
                    const double limitStockPurchasePart = config->getLimitStockPurchasePart() / HUNDRED_PERCENT;
                    const double limitByTurnoverPercent = config->getLimitByTurnoverPercent() / HUNDRED_PERCENT;

                    cost = qMin(
                        totalCost * limitStockPurchasePart,
                        tradingInfo.expectedCost * limitByTurnoverPercent // tradingInfo.expectedCost == stock.meta.turnover
                    );
                }
                else
                {
                    const double limitStockPurchasePart = config->getLimitStockPurchasePart() / HUNDRED_PERCENT;

                    cost = totalCost * limitStockPurchasePart;
                }

                amountOfLots = qMin(qRound64(cost / lotPrice), static_cast<qint64>(money / lotPriceWithCommission));
            }
            else
            {
                amountOfLots = money / lotPriceWithCommission;
            }

            if (amountOfLots > 0)
            {
                tradingInfo.expectedCost  = amountOfLots * lotPrice;
                money                    -= amountOfLots * lotPriceWithCommission;

                res.insert(instrumentId, tradingInfo);
            }
        }
    }
}

void DecisionMaker::calculateTotalCostAndMoney(const Portfolio& portfolio, double& totalCost, double& money)
{
    totalCost = 0.0;
    money     = 0.0;

    for (const PortfolioCategoryItem& category : portfolio.positions)
    {
        for (const PortfolioItem& item : category.items)
        {
            if (item.instrumentId == RUBLE_UID)
            {
                money = item.cost;
            }

            totalCost += item.cost;
        }
    }
}
