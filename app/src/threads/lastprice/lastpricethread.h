#pragma once



#include "src/threads/lastprice/ilastpricethread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"



class LastPriceThread : public ILastPriceThread
{
    Q_OBJECT

public:
    explicit LastPriceThread(IStocksStorage* stocksStorage, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~LastPriceThread();

    LastPriceThread(const LastPriceThread& another)            = delete;
    LastPriceThread& operator=(const LastPriceThread& another) = delete;

    void run() override;

private:
    IStocksStorage* mStocksStorage;
    IGrpcClient*    mGrpcClient;
};
