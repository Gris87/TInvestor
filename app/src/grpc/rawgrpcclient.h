#pragma once



#include "src/grpc/irawgrpcclient.h"



class RawGrpcClient : public IRawGrpcClient
{
public:
    explicit RawGrpcClient();
    ~RawGrpcClient();

    RawGrpcClient(const RawGrpcClient& another)            = delete;
    RawGrpcClient& operator=(const RawGrpcClient& another) = delete;
};
