#include "src/threads/orderbook/orderbookthread.h"

#include <QDebug>



OrderBookThread::OrderBookThread(IGrpcClient* grpcClient, QObject* parent) :
    IOrderBookThread(parent),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create OrderBookThread";
}

OrderBookThread::~OrderBookThread()
{
    qDebug() << "Destroy OrderBookThread";
}

void OrderBookThread::run()
{
    qDebug() << "Running OrderBookThread";

    std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mStock->meta.uid);

    if (tinkoffOrderBook != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        createMarketDataStream();

        if (mGrpcClient->subscribeOrderBook(mMarketDataStream, mStock->meta.uid))
        {
            while (true)
            {
                std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse =
                    mGrpcClient->readMarketDataStream(mMarketDataStream);

                if (QThread::currentThread()->isInterruptionRequested() || marketDataResponse == nullptr)
                {
                    break;
                }

                if (marketDataResponse->has_orderbook())
                {
                    emit orderBookChanged(marketDataResponse->orderbook());
                }
            }
        }

        mGrpcClient->finishMarketDataStream(mMarketDataStream);
        mMarketDataStream = nullptr;
    }

    qDebug() << "Finish OrderBookThread";
}

void OrderBookThread::setStock(Stock* stock)
{
    mStock = stock;
}

void OrderBookThread::terminateThread()
{
    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

void OrderBookThread::createMarketDataStream()
{
    mMarketDataStream = mGrpcClient->createMarketDataStream();
}
