#pragma once



#include <QObject>

#pragma warning(push)
#pragma warning(disable : 4100 4189 4267)
#include "messages/generated/instruments.grpc.pb.h"
#include "messages/generated/marketdata.grpc.pb.h"
#include "messages/generated/users.grpc.pb.h"
#pragma warning(pop)



namespace tinkoff
{
using namespace tinkoff::public_::invest::api::contract::v1;
}



class IGrpcClient : public QObject
{
    Q_OBJECT

public:
    explicit IGrpcClient(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    virtual ~IGrpcClient() = default;

    IGrpcClient(const IGrpcClient& another)            = delete;
    IGrpcClient& operator=(const IGrpcClient& another) = delete;

    virtual std::shared_ptr<tinkoff::GetInfoResponse>     getUserInfo() = 0;
    virtual std::shared_ptr<tinkoff::GetAccountsResponse> getAccounts() = 0;
    virtual std::shared_ptr<tinkoff::SharesResponse>      findStocks()  = 0;
    virtual std::shared_ptr<tinkoff::GetCandlesResponse>  getCandles(const QString& uid, qint64 from, qint64 to) = 0;

signals:
    void authFailed();
};
