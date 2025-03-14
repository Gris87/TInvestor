#pragma once



#include "src/grpc/irawgrpcclient.h"

#include <gmock/gmock.h>



class RawGrpcClientMock : public IRawGrpcClient
{
public:
    explicit RawGrpcClientMock() :
        IRawGrpcClient()
    {
    }
    ~RawGrpcClientMock() override = default;

    RawGrpcClientMock(const RawGrpcClientMock& another)            = delete;
    RawGrpcClientMock& operator=(const RawGrpcClientMock& another) = delete;
};
