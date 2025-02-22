#pragma once



#include "src/threads/pricecollect/ipricecollectthread.h"

#include "src/grpc/igrpcclient.h"



class PriceCollectThread : public IPriceCollectThread
{
    Q_OBJECT

public:
    explicit PriceCollectThread(IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~PriceCollectThread();

    PriceCollectThread(const PriceCollectThread& another)            = delete;
    PriceCollectThread& operator=(const PriceCollectThread& another) = delete;

    void run() override;

private:
    IGrpcClient*  mGrpcClient;
};
