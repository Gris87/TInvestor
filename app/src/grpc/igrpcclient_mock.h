#pragma once



#include "src/grpc/igrpcclient.h"

#include <gmock/gmock.h>



class GrpcClientMock : public IGrpcClient
{
public:
    explicit GrpcClientMock(QObject* parent = nullptr) :
        IGrpcClient(parent)
    {
    }
    ~GrpcClientMock() override = default;

    GrpcClientMock(const GrpcClientMock& another)            = delete;
    GrpcClientMock& operator=(const GrpcClientMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<tinkoff::GetInfoResponse>, getUserInfo, (), (override));
    MOCK_METHOD(std::shared_ptr<tinkoff::GetAccountsResponse>, getAccounts, (), (override));
};
