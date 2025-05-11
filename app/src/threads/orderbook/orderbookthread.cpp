#include "src/threads/orderbook/orderbookthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



OrderBookThread::OrderBookThread(IGrpcClient* grpcClient, QObject* parent) :
    IOrderBookThread(parent),
    mGrpcClient(grpcClient),
    mStock(),
    mMarketDataStream()
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

    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mStock->meta.uid);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffOrderBook != nullptr)
    {
        handleGetOrderBookResponse(tinkoffOrderBook);

        createMarketDataStream();

        if (mGrpcClient->subscribeOrderBook(mMarketDataStream, mStock->meta.uid))
        {
            while (true)
            {
                const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse =
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
        mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
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
    orderBook.price     = mStock->lastPrice();

    orderBook.bids.resizeForOverwrite(tinkoffOrderBook->bids_size());
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
    orderBook.price     = mStock->lastPrice();

    orderBook.bids.resizeForOverwrite(tinkoffOrderBook.bids_size());
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
