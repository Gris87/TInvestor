#include "src/threads/orderbook/orderbookthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



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
        handleGetOrderBookResponse(tinkoffOrderBook);

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
                    handleOrderBook(marketDataResponse->orderbook());
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
        mGrpcClient->unsubscribeOrderBook(mMarketDataStream);
        mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

void OrderBookThread::createMarketDataStream()
{
    mMarketDataStream = mGrpcClient->createMarketDataStream();
}

void OrderBookThread::handleGetOrderBookResponse(const std::shared_ptr<tinkoff::GetOrderBookResponse>& tinkoffOrderBook)
{
    OrderBook orderBook;

    orderBook.timestamp = QDateTime::currentMSecsSinceEpoch();
    orderBook.price     = !mStock->operational.detailedData.isEmpty() ? mStock->operational.detailedData.last().price
                                                                      : (!mStock->data.isEmpty() ? mStock->data.last().price : 0);

    orderBook.bids.reserve(tinkoffOrderBook->bids_size());
    orderBook.bids.resizeForOverwrite(tinkoffOrderBook->bids_size());
    orderBook.asks.reserve(tinkoffOrderBook->asks_size());
    orderBook.asks.resizeForOverwrite(tinkoffOrderBook->asks_size());

    OrderBookData* bids = orderBook.bids.data();
    OrderBookData* asks = orderBook.asks.data();

    for (int i = 0; i < tinkoffOrderBook->bids_size(); ++i)
    {
        const tinkoff::Order& tinkoffOrder = tinkoffOrderBook->bids(i);

        bids[i].quantity = tinkoffOrder.quantity();
        bids[i].price    = quotationToFloat(tinkoffOrder.price());
    }

    for (int i = 0; i < tinkoffOrderBook->asks_size(); ++i)
    {
        const tinkoff::Order& tinkoffOrder = tinkoffOrderBook->asks(i);

        asks[i].quantity = tinkoffOrder.quantity();
        asks[i].price    = quotationToFloat(tinkoffOrder.price());
    }

    emit orderBookChanged(orderBook);
}

void OrderBookThread::handleOrderBook(const tinkoff::OrderBook& tinkoffOrderBook)
{
    OrderBook orderBook;

    orderBook.timestamp = timeToTimestamp(tinkoffOrderBook.time());
    orderBook.price     = !mStock->operational.detailedData.isEmpty() ? mStock->operational.detailedData.last().price
                                                                      : (!mStock->data.isEmpty() ? mStock->data.last().price : 0);

    orderBook.bids.reserve(tinkoffOrderBook.bids_size());
    orderBook.bids.resizeForOverwrite(tinkoffOrderBook.bids_size());
    orderBook.asks.reserve(tinkoffOrderBook.asks_size());
    orderBook.asks.resizeForOverwrite(tinkoffOrderBook.asks_size());

    OrderBookData* bids = orderBook.bids.data();
    OrderBookData* asks = orderBook.asks.data();

    for (int i = 0; i < tinkoffOrderBook.bids_size(); ++i)
    {
        const tinkoff::Order& tinkoffOrder = tinkoffOrderBook.bids(i);

        bids[i].quantity = tinkoffOrder.quantity();
        bids[i].price    = quotationToFloat(tinkoffOrder.price());
    }

    for (int i = 0; i < tinkoffOrderBook.asks_size(); ++i)
    {
        const tinkoff::Order& tinkoffOrder = tinkoffOrderBook.asks(i);

        asks[i].quantity = tinkoffOrder.quantity();
        asks[i].price    = quotationToFloat(tinkoffOrder.price());
    }

    emit orderBookChanged(orderBook);
}
