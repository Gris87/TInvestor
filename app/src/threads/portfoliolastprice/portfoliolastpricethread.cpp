#include "src/threads/portfoliolastprice/portfoliolastpricethread.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 SLEEP_DELAY  = 5LL * MS_IN_SECOND; // 5 seconds



PortfolioLastPriceThread::PortfolioLastPriceThread(
    IStocksStorage* stocksStorage, ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent
) :
    IPortfolioLastPriceThread(parent),
    mStocksStorage(stocksStorage),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mMarketDataStream(),
    mNeedToRebuildStocksMap()
{
    qDebug() << "Create PortfolioLastPriceThread";
}

PortfolioLastPriceThread::~PortfolioLastPriceThread()
{
    qDebug() << "Destroy PortfolioLastPriceThread";
}

void PortfolioLastPriceThread::run()
{
    qDebug() << "Running PortfolioLastPriceThread";

    QStringList stocks;

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        stocks = getStockUIDs();

        if (!stocks.empty())
        {
            break;
        }

        if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
        {
            break;
        }
    }

    if (stocks.empty())
    {
        return;
    }

    QMap<QString, Stock*> stocksMap;
    mNeedToRebuildStocksMap = true;

    createMarketDataStream();

    if (mGrpcClient->subscribeLastPrices(mMarketDataStream, stocks))
    {
        while (true)
        {
            if (mNeedToRebuildStocksMap)
            {
                stocksMap               = buildStocksMap();
                mNeedToRebuildStocksMap = false;
            }

            const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse =
                mGrpcClient->readMarketDataStream(mMarketDataStream);

            if (QThread::currentThread()->isInterruptionRequested() || marketDataResponse == nullptr)
            {
                break;
            }

            if (marketDataResponse->has_last_price())
            {
                const tinkoff::LastPrice& lastPriceResp = marketDataResponse->last_price();

                StockOperationalData stockData; // NOLINT(cppcoreguidelines-pro-type-member-init)

                stockData.timestamp = timeToTimestamp(lastPriceResp.time());
                stockData.price     = quotationToFloat(lastPriceResp.price());

                const QString instrumentId = QString::fromStdString(lastPriceResp.instrument_uid());

                Stock* stock = stocksMap[instrumentId];

                const QMutexLocker lock(stock->mutex);
                stock->operational.detailedData.append(stockData);

                emit lastPriceChanged(instrumentId);
            }
        }
    }

    mGrpcClient->finishMarketDataStream(mMarketDataStream);
    mMarketDataStream = nullptr;

    qDebug() << "Finish PortfolioLastPriceThread";
}

QStringList PortfolioLastPriceThread::getStockUIDs()
{
    QStringList res;

    const QMutexLocker   lock(mStocksStorage->getMutex());
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    res.resizeForOverwrite(stocks.size());

    for (int i = 0; i < stocks.size(); ++i)
    {
        res[i] = stocks.at(i)->meta.uid;
    }

    return res;
}

QMap<QString, Stock*> PortfolioLastPriceThread::buildStocksMap()
{
    QMap<QString, Stock*> res; // UID => Stock

    const QMutexLocker   lock(mStocksStorage->getMutex());
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    for (Stock* stock : stocks)
    {
        res[stock->meta.uid] = stock;
    }

    return res;
}

void PortfolioLastPriceThread::stocksChanged()
{
    if (mMarketDataStream != nullptr)
    {
        if (mGrpcClient->unsubscribeLastPrices(mMarketDataStream))
        {
            mNeedToRebuildStocksMap = true;
            mGrpcClient->subscribeLastPrices(mMarketDataStream, getStockUIDs());
        }
    }
}

void PortfolioLastPriceThread::terminateThread()
{
    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

void PortfolioLastPriceThread::createMarketDataStream()
{
    mMarketDataStream = mGrpcClient->createMarketDataStream();
}
