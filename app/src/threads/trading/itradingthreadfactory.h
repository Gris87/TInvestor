#pragma once



#include "src/grpc/igrpcclient.h"
#include "src/threads/trading/itradingthread.h"



class ITradingThreadFactory
{
public:
    ITradingThreadFactory()          = default;
    virtual ~ITradingThreadFactory() = default;

    ITradingThreadFactory(const ITradingThreadFactory& another)            = delete;
    ITradingThreadFactory& operator=(const ITradingThreadFactory& another) = delete;

    virtual ITradingThread* newInstance(IGrpcClient* grpcClient, QObject* parent) const = 0;
};
