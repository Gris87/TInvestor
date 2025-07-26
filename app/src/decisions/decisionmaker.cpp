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
    qint64 timestamp, const Portfolio& portfolio, const QList<Stock*>& stocks, int keepMoney, bool dateRange
)
{
    InstrumentsForTrading res;

    if (mConfig->isUseSchedule())
    {
        const QTime time      = QDateTime::fromMSecsSinceEpoch(timestamp).time(); // TODO: Moscow time?
        const QTime startTime = QTime(mConfig->getScheduleStartHour(), mConfig->getScheduleStartMinute());
        const QTime endTime   = QTime(mConfig->getScheduleEndHour(), mConfig->getScheduleEndMinute());

        if (time < startTime || time > endTime)
        {
            return res;
        }
    }

    QList<Stock*> stocksForBuy;
    QList<Stock*> stocksForSell;

    updateStocksMap(stocks);
    splitStocks(portfolio, stocks, stocksForBuy, stocksForSell);

    makeBuyDecisions(timestamp, portfolio, stocksForBuy, keepMoney, dateRange, res);
    makeSellDecisions(timestamp, stocksForSell, dateRange, res);

    return res;
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
    const Portfolio& portfolio, const QList<Stock*>& stocks, QList<Stock*>& stocksForBuy, QList<Stock*>& stocksForSell
)
{
    mUserStorage->readLock();
    const bool qualifiedUser = mUserStorage->isQualified();
    mUserStorage->readUnlock();

    QSet<QString> existingStocks;

    for (const PortfolioCategoryItem& category : portfolio.positions)
    {
        for (const PortfolioItem& item : category.items)
        {
            if (mStocksMap.contains(item.instrumentId))
            {
                existingStocks.insert(item.instrumentId);
            }
        }
    }

    for (Stock* stock : stocks)
    {
        stock->readLock();

        if (!existingStocks.contains(stock->meta.instrumentId))
        {
            if (qualifiedUser || !stock->meta.forQualInvestorFlag)
            {
                stocksForBuy.append(stock);
            }
        }
        else
        {
            stocksForSell.append(stock);
        }

        stock->readUnlock();
    }
}

struct MakeBuyDecisionsInfo
{
    explicit MakeBuyDecisionsInfo(qint64 _timestamp, bool _dateRange, QList<IActionDecision*>* _buyDecisions) :
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

    qint64                       timestamp;
    bool                         dateRange;
    QList<IActionDecision*>*     buyDecisions;
    QList<InstrumentsForTrading> results;
};

static void
makeBuyDecisionsForParallel(QThread* parentThread, int threadId, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    MakeBuyDecisionsInfo* makeBuyDecisionsInfo = reinterpret_cast<MakeBuyDecisionsInfo*>(additionalArgs);

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

        for (int j = 0; j < buyDecisionsSize && !parentThread->isInterruptionRequested(); ++j)
        {
            const QString cause = buyDecisionsArray[j]->makeDecision(stock, dateRange, dataIndex, price);

            if (cause != "")
            {
                TradingInfo tradingInfo;

                tradingInfo.price        = price;
                tradingInfo.expectedCost = stock->meta.turnover;
                tradingInfo.cause        = cause;

                resultsArray[threadId][stock->meta.instrumentId] = tradingInfo;

                break;
            }
        }

        stock->readUnlock();
    }
}

void DecisionMaker::makeBuyDecisions(
    qint64                 timestamp,
    const Portfolio&       portfolio,
    QList<Stock*>&         stocksForBuy,
    int                    keepMoney,
    bool                   dateRange,
    InstrumentsForTrading& res
)
{
    double totalCost;
    double money;

    calculateTotalCostAndMoney(portfolio, totalCost, money);
    money -= keepMoney;

    if (money <= 0)
    {
        return;
    }

    MakeBuyDecisionsInfo makeBuyDecisionsInfo(timestamp, dateRange, &mBuyDecisions);
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

            qint64 amountOfLots;

            if (mConfig->isLimitStockPurchase())
            {
                double cost;

                if (mConfig->isLimitByTurnover())
                {
                    cost = qMin(
                        totalCost * mConfig->getLimitStockPurchasePart() / HUNDRED_PERCENT,
                        tradingInfo.expectedCost * // tradingInfo.expectedCost == stock.meta.turnover
                            mConfig->getLimitByTurnoverPercent() / HUNDRED_PERCENT
                    );
                }
                else
                {
                    cost = totalCost * mConfig->getLimitStockPurchasePart() / HUNDRED_PERCENT;
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
    explicit MakeSellDecisionsInfo(qint64 _timestamp, bool _dateRange, QList<IActionDecision*>* _sellDecisions) :
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

    qint64                       timestamp;
    bool                         dateRange;
    QList<IActionDecision*>*     sellDecisions;
    QList<InstrumentsForTrading> results;
};

static void
makeSellDecisionsForParallel(QThread* parentThread, int threadId, QList<Stock*>& stocks, int start, int end, void* additionalArgs)
{
    MakeSellDecisionsInfo* makeSellDecisionsInfo = reinterpret_cast<MakeSellDecisionsInfo*>(additionalArgs);

    const qint64           timestamp          = makeSellDecisionsInfo->timestamp;
    const bool             dateRange          = makeSellDecisionsInfo->dateRange;
    IActionDecision**      sellDecisionsArray = makeSellDecisionsInfo->sellDecisions->data();
    const int              sellDecisionsSize  = makeSellDecisionsInfo->sellDecisions->size();
    InstrumentsForTrading* resultsArray       = makeSellDecisionsInfo->results.data();

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

        for (int j = 0; j < sellDecisionsSize && !parentThread->isInterruptionRequested(); ++j)
        {
            const QString cause = sellDecisionsArray[j]->makeDecision(stock, dateRange, dataIndex, price);

            if (cause != "")
            {
                TradingInfo tradingInfo;

                tradingInfo.price        = price;
                tradingInfo.expectedCost = 0.0;
                tradingInfo.cause        = cause;

                resultsArray[threadId][stock->meta.instrumentId] = tradingInfo;

                break;
            }
        }

        stock->readUnlock();
    }
}

void DecisionMaker::makeSellDecisions(qint64 timestamp, QList<Stock*>& stocksForSell, bool dateRange, InstrumentsForTrading& res)
{
    MakeSellDecisionsInfo makeSellDecisionsInfo(timestamp, dateRange, &mSellDecisions);
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
