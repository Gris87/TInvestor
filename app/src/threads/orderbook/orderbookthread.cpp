#include "src/threads/orderbook/orderbookthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr int ORDER_BOOK_DEPTH = 50;



OrderBookThread::OrderBookThread(IGrpcClient* grpcClient, QObject* parent) :
    IOrderBookThread(parent),
    mRwMutex(new QReadWriteLock()),
    mGrpcClient(grpcClient),
    mStock(),
    mMarketDataStream()
{
    qDebug() << "Create OrderBookThread";
}

OrderBookThread::~OrderBookThread()
{
    qDebug() << "Destroy OrderBookThread";

    delete mRwMutex;
}

void OrderBookThread::run()
{
    qDebug() << "Running OrderBookThread";

    blockSignals(false);

    const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
        mGrpcClient->getOrderBook(QThread::currentThread(), mStock->meta.instrumentId, ORDER_BOOK_DEPTH);

    if (!QThread::currentThread()->isInterruptionRequested() && tinkoffOrderBook != nullptr)
    {
        handleGetOrderBookResponse(tinkoffOrderBook);

        if (createMarketDataStream())
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

        if (mMarketDataStream != nullptr)
        {
            mGrpcClient->finishMarketDataStream(mMarketDataStream);
            mMarketDataStream = nullptr;
        }
    }

    qDebug() << "Finish OrderBookThread";
}

void OrderBookThread::setStock(Stock* stock)
{
    mStock = stock;
}

void OrderBookThread::terminateThread()
{
    blockSignals(true);

    const QReadLocker lock(mRwMutex);

    if (mMarketDataStream != nullptr)
    {
        mGrpcClient->closeWriteMarketDataStream(mMarketDataStream);
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

bool OrderBookThread::createMarketDataStream()
{
    bool res = false;

    const QWriteLocker lock(mRwMutex);

    if (!QThread::currentThread()->isInterruptionRequested())
    {
        mMarketDataStream = mGrpcClient->createMarketDataStream();

        res = mGrpcClient->subscribeOrderBook(mMarketDataStream, mStock->meta.instrumentId, ORDER_BOOK_DEPTH);
    }

    return res;
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
