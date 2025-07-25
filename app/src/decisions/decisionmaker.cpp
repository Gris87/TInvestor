#include "src/decisions/decisionmaker.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



DecisionMaker::DecisionMaker(
    IConfig*                       config,
    IUserStorage*                  userStorage,
    const QList<IActionDecision*>& buyDecisions,
    const QList<IActionDecision*>& sellDecisions
) :
    IDecisionMaker(),
    mConfig(config),
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
        QTime time      = QDateTime::fromMSecsSinceEpoch(timestamp).time(); // TODO: Moscow time?
        QTime startTime = QTime(mConfig->getScheduleStartHour(), mConfig->getScheduleStartMinute());
        QTime endTime   = QTime(mConfig->getScheduleEndHour(), mConfig->getScheduleEndMinute());

        if (time < startTime || time > endTime)
        {
            return res;
        }
    }

    QList<Stock*> stocksForBuy;
    QList<Stock*> stocksForSell;

    updateStocksMap(stocks);
    splitStocks(portfolio, stocks, stocksForBuy, stocksForSell);

    makeBuyDecisions(timestamp, stocksForBuy, keepMoney, dateRange, res);
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

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocksArray[i];
        stock->readLock();

        int   dataIndex;
        float price;

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

                tradingInfo.price = price;
                tradingInfo.cause = cause;

                resultsArray[threadId][stock->meta.instrumentId] = tradingInfo;

                break;
            }
        }

        stock->readUnlock();
    }
}

void DecisionMaker::makeBuyDecisions(
    qint64 timestamp, QList<Stock*>& stocksForBuy, int /*keepMoney*/, bool dateRange, InstrumentsForTrading& res
)
{
    MakeBuyDecisionsInfo makeBuyDecisionsInfo(timestamp, dateRange, &mBuyDecisions);
    processInParallel(stocksForBuy, makeBuyDecisionsForParallel, &makeBuyDecisionsInfo);

    for (const InstrumentsForTrading& result : makeBuyDecisionsInfo.results)
    {
        for (auto it = result.constBegin(); it != result.constEnd(); ++it)
        {
            TradingInfo tradingInfo = it.value();

            // TODO: Make correct cost calculation
            tradingInfo.expectedCost = 1000.0; // NOLINT(readability-magic-numbers)

            res.insert(it.key(), tradingInfo);
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

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocksArray[i];
        stock->readLock();

        int   dataIndex;
        float price;

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

    for (const InstrumentsForTrading& result : makeSellDecisionsInfo.results)
    {
        res.insert(result);
    }
}
