#include "src/threads/lastprice/lastpricethread.h"

#include <QDebug>
#include <QMutexLocker>



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

    QStringList stocks = getStockUIDs();

    std::shared_ptr<MarketDataStream> marketDataStream = mGrpcClient->createMarketDataStream();
    mGrpcClient->subscribeLastPrices(marketDataStream, stocks);

    while (true)
    {
        std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse = mGrpcClient->readMarketDataStream(marketDataStream);

        if (QThread::currentThread()->isInterruptionRequested() || marketDataResponse == nullptr)
        {
            break;
        }

        if (marketDataResponse->has_last_price())
        {
            const tinkoff::LastPrice& lastPriceResp = marketDataResponse->last_price();

            qInfo() << lastPriceResp.figi();
            qInfo() << lastPriceResp.price().units();
            qInfo() << lastPriceResp.price().nano();
            qInfo() << lastPriceResp.time().seconds();
            qInfo() << lastPriceResp.time().nanos();
            qInfo() << lastPriceResp.instrument_uid();
            qInfo() << lastPriceResp.last_price_type();
        }
    }

    mGrpcClient->finishMarketDataStream(marketDataStream);

    qDebug() << "Finish LastPriceThread";
}

QStringList LastPriceThread::getStockUIDs()
{
    QStringList res;

    QMutexLocker  lock(mStocksStorage->getMutex());
    QList<Stock>* stocks = mStocksStorage->getStocks();

    for (int i = 0; i < stocks->size(); ++i)
    {
        res.append(stocks->at(i).meta.uid);
    }

    return res;
}
