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
    ~IGrpcClient() override = default;

    IGrpcClient(const IGrpcClient& another)            = delete;
    IGrpcClient& operator=(const IGrpcClient& another) = delete;

    virtual std::shared_ptr<tinkoff::GetInfoResponse>     getUserInfo(QThread* parentThread)    = 0;
    virtual std::shared_ptr<tinkoff::GetAccountsResponse> getAccounts(QThread* parentThread)    = 0;
    virtual std::shared_ptr<tinkoff::SharesResponse>      findStocks(QThread* parentThread)     = 0;
    virtual std::shared_ptr<tinkoff::BondsResponse>       findBonds(QThread* parentThread)      = 0;
    virtual std::shared_ptr<tinkoff::CurrenciesResponse>  findCurrencies(QThread* parentThread) = 0;
    virtual std::shared_ptr<tinkoff::EtfsResponse>        findEtfs(QThread* parentThread)       = 0;
    virtual std::shared_ptr<tinkoff::FuturesResponse>     findFutures(QThread* parentThread)    = 0;
    virtual std::shared_ptr<tinkoff::GetCandlesResponse>
    getCandles(QThread* parentThread, const QString& uid, qint64 from, qint64 to)                                  = 0;
    virtual std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBook(QThread* parentThread, const QString& uid) = 0;
    virtual std::shared_ptr<tinkoff::OperationsResponse>   getOperations(QThread* parentThread, const QString& accountId) = 0;

    virtual std::shared_ptr<MarketDataStream>              createMarketDataStream()                                = 0;
    virtual bool subscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream, const QStringList& uids) = 0;
    virtual bool unsubscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream)                        = 0;
    virtual bool subscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream, const QString& uid)       = 0;
    virtual bool unsubscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream)                         = 0;
    virtual std::shared_ptr<tinkoff::MarketDataResponse>
                 readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)       = 0;
    virtual bool closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) = 0;
    virtual void finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)     = 0;

signals:
    void authFailed(
        grpc::StatusCode   errorCode,
        const QString&     errorCodeString,
        const std::string& errorMessage,
        const std::string& errorDetails
    );
};
