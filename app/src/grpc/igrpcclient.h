#pragma once



#include <QObject>

#include <QThread>

#include "src/grpc/irawgrpcclient.h"



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
    virtual bool subscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream, const QStringList& uids)          = 0;
    virtual bool unsubscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream)                                 = 0;
    virtual std::shared_ptr<tinkoff::MarketDataResponse>
                 readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)       = 0;
    virtual bool closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) = 0;
    virtual void finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)     = 0;

signals:
    void authFailed();
};
