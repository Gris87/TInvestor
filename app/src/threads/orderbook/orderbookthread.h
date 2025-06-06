#pragma once



#include "src/threads/orderbook/iorderbookthread.h"

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

    void createMarketDataStream();
    void handleGetOrderBookResponse(const std::shared_ptr<tinkoff::GetOrderBookResponse>& tinkoffOrderBook);
    void handleOrderBook(const tinkoff::OrderBook& tinkoffOrderBook);

private:
    IGrpcClient*                      mGrpcClient;
    Stock*                            mStock;
    std::shared_ptr<MarketDataStream> mMarketDataStream;
};
