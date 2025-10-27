#include "src/threads/orderbook/orderbookthread.h"

#include <QDebug>

#include "src/grpc/utils.h"



constexpr int    ORDER_BOOK_DEPTH = 50;
constexpr qint64 MS_IN_SECOND     = 1000LL;
constexpr qint64 SLEEP_DELAY      = 5LL * MS_IN_SECOND; // 5 seconds



OrderBookThread::OrderBookThread(ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent) :
    IOrderBookThread(parent),
    mRwMutex(new QReadWriteLock()),
    mTimeUtils(timeUtils),
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

// NOLINTBEGIN(readability-function-cognitive-complexity)
void OrderBookThread::run()
{
    qDebug() << "Running OrderBookThread";

    blockSignals(false);

    mStock->readLock();
    const QString instrumentId = mStock->meta.instrumentId;
    mStock->readUnlock();

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        if (createMarketDataStream(instrumentId))
        {
            const std::shared_ptr<tinkoff::GetOrderBookResponse> tinkoffOrderBook =
                mGrpcClient->getOrderBook(QThread::currentThread(), instrumentId, ORDER_BOOK_DEPTH);

            if (!QThread::currentThread()->isInterruptionRequested() && tinkoffOrderBook != nullptr)
            {
                handleGetOrderBookResponse(tinkoffOrderBook);

                while (true)
                {
                    const std::shared_ptr<tinkoff::MarketDataResponse> marketDataResponse =
                        mGrpcClient->readMarketDataStream(mMarketDataStream);

                    if (QThread::currentThread()->isInterruptionRequested() || marketDataResponse == nullptr)
                    {
                        mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread());

                        break;
                    }

                    if (marketDataResponse->has_orderbook())
                    {
                        handleOrderBook(marketDataResponse->orderbook());
                    }
                }
            }
            else
            {
                mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread());
            }

            const QWriteLocker lock(mRwMutex);

            mGrpcClient->finishMarketDataStream(mMarketDataStream);
            mMarketDataStream = nullptr;
        }
        else
        {
            if (mTimeUtils->interruptibleSleep(SLEEP_DELAY, QThread::currentThread()))
            {
                break;
            }
        }
    }

    qDebug() << "Finish OrderBookThread";
}
// NOLINTEND(readability-function-cognitive-complexity)

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
        mGrpcClient->cancelMarketDataStream(mMarketDataStream);
    }

    requestInterruption();
}

bool OrderBookThread::createMarketDataStream(const QString& instrumentId)
{
    bool res = false;

    const QWriteLocker lock(mRwMutex);

    if (!QThread::currentThread()->isInterruptionRequested())
    {
        mMarketDataStream = mGrpcClient->createMarketDataStreamForOrderBook(instrumentId, ORDER_BOOK_DEPTH);

        res = mMarketDataStream != nullptr;
    }

    return res;
}

void OrderBookThread::handleGetOrderBookResponse(const std::shared_ptr<tinkoff::GetOrderBookResponse>& tinkoffOrderBook)
{
    OrderBook orderBook;

    mStock->readLock();
    const float price = mStock->lastPrice();
    mStock->readUnlock();

    orderBook.timestamp = QDateTime::currentMSecsSinceEpoch();
    orderBook.price     = price;

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

    mStock->readLock();
    const float price = mStock->lastPrice();
    mStock->readUnlock();

    orderBook.timestamp = timeToTimestamp(tinkoffOrderBook.time());
    orderBook.price     = price;

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
