#pragma once



#include "src/grpc/igrpcclient.h"

#include <gmock/gmock.h>



class GrpcClientMock : public IGrpcClient
{
public:
    GrpcClientMock() :
        IGrpcClient()
    {
    }
    ~GrpcClientMock() override = default;

    GrpcClientMock(const GrpcClientMock& another)            = delete;
    GrpcClientMock& operator=(const GrpcClientMock& another) = delete;

    MOCK_METHOD(void, connect, (), (override));
};
