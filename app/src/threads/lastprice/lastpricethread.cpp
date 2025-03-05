#include "src/threads/lastprice/lastpricethread.h"

#include <QDebug>
#include <QMutexLocker>

#include "src/grpc/utils.h"



LastPriceThread::LastPriceThread(IStocksStorage* stocksStorage, IGrpcClient* grpcClient, QObject* parent) :
    ILastPriceThread(parent),
    mStocksStorage(stocksStorage),
    mGrpcClient(grpcClient)
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

    QStringList stocks;

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        stocks = getStockUIDs();

        if (!stocks.empty())
        {
            break;
        }

        for (int t = 0; t < 10 && !QThread::currentThread()->isInterruptionRequested(); ++t)
        {
            QThread::msleep(500);
        }
    }

    if (stocks.empty())
    {
        return;
    }

    QMap<std::string, Stock*> stocksMap;
    mNeedToRebuildStocksMap = true;

    mMarketDataStream = mGrpcClient->createMarketDataStream();
    mGrpcClient->subscribeLastPrices(mMarketDataStream, stocks);

    while (true)
    {
        if (mNeedToRebuildStocksMap)
        {
            stocksMap               = buildStocksMap();
            mNeedToRebuildStocksMap = false;
        }

        std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse = mGrpcClient->readMarketDataStream(mMarketDataStream);

        if (QThread::currentThread()->isInterruptionRequested() || marketDataResponse == nullptr)
        {
            break;
        }

        if (marketDataResponse->has_last_price())
        {
            const tinkoff::LastPrice& lastPriceResp = marketDataResponse->last_price();

            StockData stockData;

            stockData.timestamp = lastPriceResp.time().seconds() * 1000 + lastPriceResp.time().nanos() / 1000000;
            stockData.price     = quotationToFloat(lastPriceResp.price());

            Stock* stock = stocksMap[lastPriceResp.instrument_uid()];

            QMutexLocker lock(stock->mutex);
            stock->operational.detailedData.append(stockData);
        }
    }

    mGrpcClient->finishMarketDataStream(mMarketDataStream);
    mMarketDataStream = nullptr;

    qDebug() << "Finish LastPriceThread";
}

QStringList LastPriceThread::getStockUIDs()
{
    QStringList res;

    QMutexLocker   lock(mStocksStorage->getMutex());
    QList<Stock*>& stocks = mStocksStorage->getStocks();

    res.reserve(stocks.size());
    res.resizeForOverwrite(stocks.size());

    for (int i = 0; i < stocks.size(); ++i)
    {
        res[i] = stocks.at(i)->meta.uid;
    }

    return res;
}

QMap<std::string, Stock*> LastPriceThread::buildStocksMap()
{
    QMap<std::string, Stock*> res; // UID => Stock

    QMutexLocker   lock(mStocksStorage->getMutex());
    QList<Stock*>& stocks = mStocksStorage->getStocks();

    for (int i = 0; i < stocks.size(); ++i)
    {
        Stock* stock = stocks.at(i);

        res[stock->meta.uid.toStdString()] = stock;
    }

    return res;
}

void LastPriceThread::stocksChanged()
{
    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->unsubscribeLastPrices(mMarketDataStream);
        mNeedToRebuildStocksMap = true;
        mGrpcClient->subscribeLastPrices(mMarketDataStream, getStockUIDs());
    }
}

void LastPriceThread::terminateThread()
{
    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}
