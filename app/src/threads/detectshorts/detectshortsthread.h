#pragma once



#include "src/threads/detectshorts/idetectshortsthread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"



class DetectShortsThread : public IDetectShortsThread
{
    Q_OBJECT

public:
    explicit DetectShortsThread(IStocksStorage* stocksStorage, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~DetectShortsThread() override;

    DetectShortsThread(const DetectShortsThread& another)            = delete;
    DetectShortsThread& operator=(const DetectShortsThread& another) = delete;

    void run() override;

    void terminateThread() override;

private:
    QMap<QString, Stock*> buildStocksMap();

    IStocksStorage* mStocksStorage;
    IGrpcClient*    mGrpcClient;
};
