#pragma once



#include "src/grpc/igrpcclient.h"

#include "src/storage/user/iuserstorage.h"



class GrpcClient : public IGrpcClient
{
    Q_OBJECT

public:
    explicit GrpcClient(IUserStorage* userStorage, QObject* parent = nullptr);
    ~GrpcClient();

    GrpcClient(const GrpcClient& another)            = delete;
    GrpcClient& operator=(const GrpcClient& another) = delete;

    std::shared_ptr<tinkoff::GetInfoResponse>     getUserInfo() override;
    std::shared_ptr<tinkoff::GetAccountsResponse> getAccounts() override;
    std::shared_ptr<tinkoff::SharesResponse>      findStocks() override;
    std::shared_ptr<tinkoff::GetCandlesResponse>  getCandles(const QString& uid, qint64 from, qint64 to) override;

private:
    std::shared_ptr<grpc::CallCredentials>             mCreds;
    std::unique_ptr<tinkoff::UsersService::Stub>       mUsersService;
    std::unique_ptr<tinkoff::InstrumentsService::Stub> mInstrumentsService;
    std::unique_ptr<tinkoff::MarketDataService::Stub>  mMarketDataService;
};
