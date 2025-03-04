#pragma once



#include <QObject>

#include <QThread>

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



struct MarketDataStream
{
    grpc::ClientContext                                                                                context;
    std::unique_ptr<grpc::ClientReaderWriter<tinkoff::MarketDataRequest, tinkoff::MarketDataResponse>> stream;
};



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

    virtual std::shared_ptr<tinkoff::GetInfoResponse>     getUserInfo(QThread* parentThread) = 0;
    virtual std::shared_ptr<tinkoff::GetAccountsResponse> getAccounts(QThread* parentThread) = 0;
    virtual std::shared_ptr<tinkoff::SharesResponse>      findStocks(QThread* parentThread)  = 0;
    virtual std::shared_ptr<tinkoff::GetCandlesResponse>
                                              getCandles(QThread* parentThread, const QString& uid, qint64 from, qint64 to) = 0;
    virtual std::shared_ptr<MarketDataStream> createMarketDataStream()                                                      = 0;
    virtual void subscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream, const QStringList& uids)          = 0;
    virtual void unsubscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream)                                 = 0;
    virtual std::shared_ptr<tinkoff::MarketDataResponse>
                 readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)       = 0;
    virtual void closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) = 0;
    virtual void finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)     = 0;

signals:
    void authFailed();
};
