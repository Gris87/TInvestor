#include "src/threads/lastprice/lastpricethread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 SLEEP_DELAY  = 5LL * MS_IN_SECOND; // 5 seconds



LastPriceThread::LastPriceThread(IStocksStorage* stocksStorage, ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent) :
    ILastPriceThread(parent),
    mStocksStorage(stocksStorage),
    mTimeUtils(timeUtils),
    mGrpcClient(grpcClient),
    mMarketDataStream(),
    mNeedToRebuildStocksMap()
{
    qDebug() << "Create LastPriceThread";
}

LastPriceThread::~LastPriceThread()
{
    qDebug() << "Destroy LastPriceThread";
}

void LastPriceThread::run()
{
    qDebug() << "Running LastPriceThread";

    blockSignals(false);

    QStringList stocks;

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        stocks = getStockUIDs();

        if (!stocks.isEmpty())
        {
            break;
        }

        if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
        {
            break;
        }
    }

    if (stocks.isEmpty())
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

                stock->writeLock();
                stock->operational.detailedData.append(stockData);
                stock->writeUnlock();

                emit lastPriceChanged(instrumentId);
            }
        }
    }

    mGrpcClient->finishMarketDataStream(mMarketDataStream);
    mMarketDataStream = nullptr;

    qDebug() << "Finish LastPriceThread";
}

QStringList LastPriceThread::getStockUIDs()
{
    QStringList res;

    mStocksStorage->readLock();
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    res.resizeForOverwrite(stocks.size());

    for (int i = 0; i < stocks.size(); ++i)
    {
        res[i] = stocks.at(i)->meta.instrumentId;
    }
    mStocksStorage->readUnlock();

    return res;
}

QMap<QString, Stock*> LastPriceThread::buildStocksMap()
{
    QMap<QString, Stock*> res; // Instrument ID => Stock

    mStocksStorage->readLock();
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    for (Stock* stock : stocks)
    {
        res[stock->meta.instrumentId] = stock;
    }
    mStocksStorage->readUnlock();

    return res;
}

void LastPriceThread::stocksChanged()
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

void LastPriceThread::terminateThread()
{
    blockSignals(true);

    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

void LastPriceThread::createMarketDataStream()
{
    mMarketDataStream = mGrpcClient->createMarketDataStream();
}
