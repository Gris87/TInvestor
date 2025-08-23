#pragma once



#include "src/threads/orderbook/iorderbookthread.h"

#include <QReadWriteLock>

#include "src/grpc/igrpcclient.h"



class OrderBookThread : public IOrderBookThread
{
    Q_OBJECT

public:
    explicit OrderBookThread(IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~OrderBookThread() override;

    OrderBookThread(const OrderBookThread& another)            = delete;
    OrderBookThread& operator=(const OrderBookThread& another) = delete;

    void run() override;

    void setStock(Stock* stock) override;
    void terminateThread() override;

    bool createMarketDataStream();

private:
    void handleGetOrderBookResponse(const std::shared_ptr<tinkoff::GetOrderBookResponse>& tinkoffOrderBook);
    void handleOrderBook(const tinkoff::OrderBook& tinkoffOrderBook);

    QReadWriteLock*                   mRwMutex;
    IGrpcClient*                      mGrpcClient;
    Stock*                            mStock;
    std::shared_ptr<MarketDataStream> mMarketDataStream;
};
