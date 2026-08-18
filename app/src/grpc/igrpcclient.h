#pragma once



#include <QObject>

#include <QThread>

#include "src/domain/quotation/quotation.h"
#include "src/grpc/irawgrpcclient.h"



enum GrpcPriority : quint8
{
    GRPC_PRIOIRITY_NORMAL,
    GRPC_PRIOIRITY_MINOR,
    GRPC_PRIOIRITY_MAJOR,
    GRPC_PRIOIRITY_CRITICAL,
    GRPC_PRIOIRITY_COUNT
};



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

    virtual std::shared_ptr<tinkoff::GetInfoResponse>     getUserInfo(QThread* parentThread) = 0;
    virtual std::shared_ptr<tinkoff::GetAccountsResponse> getAccounts(QThread* parentThread) = 0;
    virtual std::shared_ptr<tinkoff::SharesResponse>
    findStocks(QThread* parentThread, tinkoff::InstrumentStatus instrumentStatus)              = 0;
    virtual std::shared_ptr<tinkoff::BondsResponse>      findBonds(QThread* parentThread)      = 0;
    virtual std::shared_ptr<tinkoff::CurrenciesResponse> findCurrencies(QThread* parentThread) = 0;
    virtual std::shared_ptr<tinkoff::EtfsResponse>       findEtfs(QThread* parentThread)       = 0;
    virtual std::shared_ptr<tinkoff::FuturesResponse>    findFutures(QThread* parentThread)    = 0;
    virtual std::shared_ptr<tinkoff::GetCandlesResponse>
    getCandles(QThread* parentThread, const QString& instrumentId, qint64 from, qint64 to) = 0;
    virtual std::shared_ptr<tinkoff::GetOrderBookResponse>
    getOrderBook(QThread* parentThread, const QString& instrumentId, int depth)                                       = 0;
    virtual std::shared_ptr<tinkoff::PortfolioResponse> getPortfolio(QThread* parentThread, const QString& accountId) = 0;
    virtual std::shared_ptr<tinkoff::GetOperationsByCursorResponse>
    getOperations(QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor) = 0;
    virtual std::shared_ptr<tinkoff::GetMaxLotsResponse>
    getMaxLots(QThread* parentThread, const QString& accountId, const QString& instrumentId, const Quotation& price) = 0;
    virtual std::shared_ptr<tinkoff::PostOrderResponse> postOrder(
        QThread*                parentThread,
        const QString&          accountId,
        const QString&          instrumentId,
        tinkoff::OrderDirection direction,
        qint64                  quantity,
        const Quotation&        price,
        bool                    confirmMarginTrade,
        GrpcPriority            priority
    ) = 0;
    virtual std::shared_ptr<tinkoff::OrderState>
    getOrderState(QThread* parentThread, const QString& accountId, const QString& orderId) = 0;
    virtual std::shared_ptr<tinkoff::CancelOrderResponse>
    cancelOrder(QThread* parentThread, const QString& accountId, const QString& orderId) = 0;

    virtual std::shared_ptr<MarketDataStream> createMarketDataStreamForLastPrice(const QStringList& instrumentIds)       = 0;
    virtual std::shared_ptr<MarketDataStream> createMarketDataStreamForOrderBook(const QString& instrumentId, int depth) = 0;
    virtual std::shared_ptr<tinkoff::MarketDataResponse>
                 readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)   = 0;
    virtual void cancelMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) = 0;
    virtual void finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) = 0;

    virtual std::shared_ptr<OperationsStream> createOperationsStream(const QString& accountId) = 0;
    virtual std::shared_ptr<tinkoff::OperationsStreamResponse>
                 readOperationsStream(std::shared_ptr<OperationsStream>& operationsStream)   = 0;
    virtual void cancelOperationsStream(std::shared_ptr<OperationsStream>& operationsStream) = 0;
    virtual void finishOperationsStream(std::shared_ptr<OperationsStream>& operationsStream) = 0;

    virtual std::shared_ptr<PortfolioStream> createPortfolioStream(const QString& accountId)                                  = 0;
    virtual std::shared_ptr<PortfolioStream> createPortfolioStream(const QString& accountId, const QString& anotherAccountId) = 0;
    virtual std::shared_ptr<tinkoff::PortfolioStreamResponse>
                 readPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream)   = 0;
    virtual void cancelPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream) = 0;
    virtual void finishPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream) = 0;

signals:
    void authFailed(const QString& errorCodeString);
};
