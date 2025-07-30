#include "src/decisions/decisionmaker.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";

constexpr float HUNDRED_PERCENT = 100.0f;



DecisionMaker::DecisionMaker(
    IConfig*                       config,
    IInstrumentsStorage*           instrumentsStorage,
    IUserStorage*                  userStorage,
    const QList<IActionDecision*>& buyDecisions,
    const QList<IActionDecision*>& sellDecisions
) :
    IDecisionMaker(),
    mConfig(config),
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
    qint64 timestamp, const Portfolio& portfolio, const QList<Stock*>& stocks, bool autoPilot, int keepMoney, bool dateRange
)
{
    InstrumentsForTrading res;

    if (mConfig->isUseSchedule())
    {
        const QTime time        = QDateTime::fromMSecsSinceEpoch(timestamp).time(); // TODO: Moscow time?
        const int   startHour   = mConfig->getScheduleStartHour();
        const int   startMinute = mConfig->getScheduleStartMinute();
        const int   endHour     = mConfig->getScheduleEndHour();
        const int   endMinute   = mConfig->getScheduleEndMinute();
        const QTime startTime   = QTime(startHour, startMinute);
        const QTime endTime     = QTime(endHour, endMinute);

        if (time < startTime || time > endTime)
        {
            return res;
        }
    }

    IDecisionMakerConfig* decisionConfig = chooseDecisionConfig(autoPilot);

    QList<Stock*>            stocksForBuy;
    QList<StockWithAvgPrice> stocksForSell;

    updateStocksMap(stocks);
    splitStocks(portfolio, stocks, stocksForBuy, stocksForSell);

    makeBuyDecisions(decisionConfig, timestamp, portfolio, stocksForBuy, keepMoney, dateRange, res);
    makeSellDecisions(decisionConfig, timestamp, stocksForSell, dateRange, res);

    return res;
}

IDecisionMakerConfig* DecisionMaker::chooseDecisionConfig(bool autoPilot)
{
    if (mConfig->isSimulatorConfigCommon())
    {
        return mConfig->getSimulatorConfig();
    }

    if (mConfig->isAutoPilotConfigCommon() || autoPilot)
    {
        return mConfig->getAutoPilotConfig();
    }

    return mConfig->getSimulatorConfig();
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

void DecisionMaker::splitStocks(
    const Portfolio& portfolio, const QList<Stock*>& stocks, QList<Stock*>& stocksForBuy, QList<StockWithAvgPrice>& stocksForSell
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
                stocksForBuy.append(stock);
            }
        }
        else
        {
            stocksForSell.append(StockWithAvgPrice(stock, avgPrice));
        }

        stock->readUnlock();
    }
}

struct MakeBuyDecisionsInfo
{
    explicit MakeBuyDecisionsInfo(
        IDecisionMakerConfig* _decisionConfig, qint64 _timestamp, bool _dateRange, QList<IActionDecision*>* _buyDecisions
    ) :
        decisionConfig(_decisionConfig),
        timestamp(_timestamp),
        dateRange(_dateRange),
        buyDecisions(_buyDecisions)
    {
#ifndef TESTING_MODE
        const int cpuCount = QThread::idealThreadCount();
#else
        const int cpuCount = 1;
#endif

        results.resize(cpuCount);
    }

    IDecisionMakerConfig*        decisionConfig;
    qint64                       timestamp;
    bool                         dateRange;
    QList<IActionDecision*>*     buyDecisions;
    QList<InstrumentsForTrading> results;
};

static void
makeBuyDecisionsForParallel(QThread* parentThread, int threadId, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    MakeBuyDecisionsInfo* makeBuyDecisionsInfo = reinterpret_cast<MakeBuyDecisionsInfo*>(additionalArgs);

    IDecisionMakerConfig*  decisionConfig    = makeBuyDecisionsInfo->decisionConfig;
    const qint64           timestamp         = makeBuyDecisionsInfo->timestamp;
    const bool             dateRange         = makeBuyDecisionsInfo->dateRange;
    IActionDecision**      buyDecisionsArray = makeBuyDecisionsInfo->buyDecisions->data();
    const int              buyDecisionsSize  = makeBuyDecisionsInfo->buyDecisions->size();
    InstrumentsForTrading* resultsArray      = makeBuyDecisionsInfo->results.data();

    Stock** stocksArray = stocks.data();

    int   dataIndex = 0;
    float price     = 0;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocksArray[i];
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

        for (int j = 0; j < buyDecisionsSize && cause == "" && !parentThread->isInterruptionRequested(); ++j)
        {
            cause = buyDecisionsArray[j]->makeDecision(decisionConfig, stock, dateRange, dataIndex, price, 0.0f);
        }

        if (cause != "")
        {
            TradingInfo tradingInfo;

            tradingInfo.price        = price;
            tradingInfo.expectedCost = stock->meta.turnover;
            tradingInfo.cause        = cause;

            resultsArray[threadId][stock->meta.instrumentId] = tradingInfo;
        }

        stock->readUnlock();
    }
}

void DecisionMaker::makeBuyDecisions(
    IDecisionMakerConfig*  decisionConfig,
    qint64                 timestamp,
    const Portfolio&       portfolio,
    QList<Stock*>&         stocksForBuy,
    int                    keepMoney,
    bool                   dateRange,
    InstrumentsForTrading& res
)
{
    double totalCost = 0.0;
    double money     = 0.0;

    calculateTotalCostAndMoney(portfolio, totalCost, money);
    money -= keepMoney;

    if (money <= 0)
    {
        return;
    }

    MakeBuyDecisionsInfo makeBuyDecisionsInfo(decisionConfig, timestamp, dateRange, &mBuyDecisions);
    processInParallel(stocksForBuy, makeBuyDecisionsForParallel, &makeBuyDecisionsInfo);

    mUserStorage->readLock();
    const float commission = mUserStorage->getCommission() / HUNDRED_PERCENT;
    mUserStorage->readUnlock();

    for (const InstrumentsForTrading& result : std::as_const(makeBuyDecisionsInfo.results))
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

            if (mConfig->isLimitStockPurchase())
            {
                double cost = 0.0;

                if (mConfig->isLimitByTurnover())
                {
                    const double limitStockPurchasePart = mConfig->getLimitStockPurchasePart() / HUNDRED_PERCENT;
                    const double limitByTurnoverPercent = mConfig->getLimitByTurnoverPercent() / HUNDRED_PERCENT;

                    cost = qMin(
                        totalCost * limitStockPurchasePart,
                        tradingInfo.expectedCost * limitByTurnoverPercent // tradingInfo.expectedCost == stock.meta.turnover
                    );
                }
                else
                {
                    const double limitStockPurchasePart = mConfig->getLimitStockPurchasePart() / HUNDRED_PERCENT;

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

struct MakeSellDecisionsInfo
{
    explicit MakeSellDecisionsInfo(
        IDecisionMakerConfig* _decisionConfig, qint64 _timestamp, bool _dateRange, QList<IActionDecision*>* _sellDecisions
    ) :
        decisionConfig(_decisionConfig),
        timestamp(_timestamp),
        dateRange(_dateRange),
        sellDecisions(_sellDecisions)
    {
#ifndef TESTING_MODE
        const int cpuCount = QThread::idealThreadCount();
#else
        const int cpuCount = 1;
#endif

        results.resize(cpuCount);
    }

    IDecisionMakerConfig*        decisionConfig;
    qint64                       timestamp;
    bool                         dateRange;
    QList<IActionDecision*>*     sellDecisions;
    QList<InstrumentsForTrading> results;
};

static void makeSellDecisionsForParallel(
    QThread* parentThread, int threadId, QList<StockWithAvgPrice>& stocks, int start, int end, void* additionalArgs
)
{
    MakeSellDecisionsInfo* makeSellDecisionsInfo = reinterpret_cast<MakeSellDecisionsInfo*>(additionalArgs);

    IDecisionMakerConfig*  decisionConfig     = makeSellDecisionsInfo->decisionConfig;
    const qint64           timestamp          = makeSellDecisionsInfo->timestamp;
    const bool             dateRange          = makeSellDecisionsInfo->dateRange;
    IActionDecision**      sellDecisionsArray = makeSellDecisionsInfo->sellDecisions->data();
    const int              sellDecisionsSize  = makeSellDecisionsInfo->sellDecisions->size();
    InstrumentsForTrading* resultsArray       = makeSellDecisionsInfo->results.data();

    StockWithAvgPrice* stocksArray = stocks.data();

    int   dataIndex = 0;
    float price     = 0;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock    = stocksArray[i].stock;
        float  avgPrice = stocksArray[i].avgPrice;

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

        for (int j = 0; j < sellDecisionsSize && cause == "" && !parentThread->isInterruptionRequested(); ++j)
        {
            cause = sellDecisionsArray[j]->makeDecision(decisionConfig, stock, dateRange, dataIndex, price, avgPrice);
        }

        if (cause != "")
        {
            TradingInfo tradingInfo;

            tradingInfo.price        = price;
            tradingInfo.expectedCost = 0.0;
            tradingInfo.cause        = cause;

            resultsArray[threadId][stock->meta.instrumentId] = tradingInfo;
        }

        stock->readUnlock();
    }
}

void DecisionMaker::makeSellDecisions(
    IDecisionMakerConfig*     decisionConfig,
    qint64                    timestamp,
    QList<StockWithAvgPrice>& stocksForSell,
    bool                      dateRange,
    InstrumentsForTrading&    res
)
{
    MakeSellDecisionsInfo makeSellDecisionsInfo(decisionConfig, timestamp, dateRange, &mSellDecisions);
    processInParallel(stocksForSell, makeSellDecisionsForParallel, &makeSellDecisionsInfo);

    for (const InstrumentsForTrading& result : std::as_const(makeSellDecisionsInfo.results))
    {
        res.insert(result);
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
