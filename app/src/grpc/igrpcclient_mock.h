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

    MOCK_METHOD(std::shared_ptr<tinkoff::GetInfoResponse>, getUserInfo, (QThread * parentThread), (override));
    MOCK_METHOD(std::shared_ptr<tinkoff::GetAccountsResponse>, getAccounts, (QThread * parentThread), (override));
    MOCK_METHOD(std::shared_ptr<tinkoff::SharesResponse>, findStocks, (QThread * parentThread), (override));
    MOCK_METHOD(
        std::shared_ptr<tinkoff::GetCandlesResponse>,
        getCandles,
        (QThread * parentThread, const QString& uid, qint64 from, qint64 to),
        (override)
    );
};
