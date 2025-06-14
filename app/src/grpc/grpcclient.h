#pragma once



#include "src/grpc/igrpcclient.h"

#include "src/storage/user/iuserstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class GrpcClient : public IGrpcClient
{
    Q_OBJECT

public:
    explicit GrpcClient(
        IUserStorage* userStorage, IRawGrpcClient* rawGrpcClient, ITimeUtils* timeUtils, QObject* parent = nullptr
    );
    ~GrpcClient() override;

    GrpcClient(const GrpcClient& another)            = delete;
    GrpcClient& operator=(const GrpcClient& another) = delete;

    template<typename REQ_T, typename RESP_T, typename SERVICE_T>
    RESP_T repeatRequest(
        QThread*     parentThread,
        grpc::Status action(
            IRawGrpcClient*      rawGrpcClient,
            const SERVICE_T&     service,
            grpc::ClientContext* context,
            const REQ_T&         req,
            const RESP_T&        resp
        ),
        const SERVICE_T&     service,
        grpc::ClientContext* context,
        const REQ_T&         req,
        const RESP_T&        resp
    )
    {
        bool running = true;

        while (running)
        {
            const grpc::Status status = action(mRawGrpcClient, service, context, req, resp);

            if (!parentThread->isInterruptionRequested() && !status.ok())
            {
                if (status.error_code() == grpc::StatusCode::RESOURCE_EXHAUSTED)
                {
                    if (mTimeUtils->interruptibleSleep(5000, parentThread)) // 5 seconds // NOLINT(readability-magic-numbers)
                    {
                        return nullptr;
                    }

                    continue;
                }

                if (status.error_code() != grpc::StatusCode::CANCELLED && status.error_code() != grpc::StatusCode::NOT_FOUND)
                {
                    emitAuthFailed(status);
                }

                return nullptr;
            }

            running = false;
        }

        return resp;
    }

    std::shared_ptr<tinkoff::GetInfoResponse>     getUserInfo(QThread* parentThread) override;
    std::shared_ptr<tinkoff::GetAccountsResponse> getAccounts(QThread* parentThread) override;
    std::shared_ptr<tinkoff::SharesResponse>
    findStocks(QThread* parentThread, tinkoff::InstrumentStatus instrumentStatus) override;
    std::shared_ptr<tinkoff::BondsResponse>      findBonds(QThread* parentThread) override;
    std::shared_ptr<tinkoff::CurrenciesResponse> findCurrencies(QThread* parentThread) override;
    std::shared_ptr<tinkoff::EtfsResponse>       findEtfs(QThread* parentThread) override;
    std::shared_ptr<tinkoff::FuturesResponse>    findFutures(QThread* parentThread) override;
    std::shared_ptr<tinkoff::GetCandlesResponse>
    getCandles(QThread* parentThread, const QString& instrumentId, qint64 from, qint64 to) override;
    std::shared_ptr<tinkoff::GetOrderBookResponse>
    getOrderBook(QThread* parentThread, const QString& instrumentId, int depth) override;
    std::shared_ptr<tinkoff::PortfolioResponse> getPortfolio(QThread* parentThread, const QString& accountId) override;
    std::shared_ptr<tinkoff::PositionsResponse> getPositions(QThread* parentThread, const QString& accountId) override;
    std::shared_ptr<tinkoff::GetOperationsByCursorResponse>
    getOperations(QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor) override;
    std::shared_ptr<tinkoff::GetMaxLotsResponse>
    getMaxLots(QThread* parentThread, const QString& accountId, const QString& instrumentId, const Quotation& price) override;
    std::shared_ptr<tinkoff::PostOrderResponse> postOrder(
        QThread*                parentThread,
        const QString&          accountId,
        const QString&          instrumentId,
        tinkoff::OrderDirection direction,
        qint64                  quantity,
        const Quotation&        price
    ) override;
    std::shared_ptr<tinkoff::OrderState>
    getOrderState(QThread* parentThread, const QString& accountId, const QString& orderId) override;
    std::shared_ptr<tinkoff::CancelOrderResponse>
    cancelOrder(QThread* parentThread, const QString& accountId, const QString& orderId) override;

    std::shared_ptr<MarketDataStream> createMarketDataStream() override;
    bool subscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream, const QStringList& instrumentIds) override;
    bool unsubscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    bool subscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream, const QString& instrumentId, int depth) override;
    bool unsubscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    std::shared_ptr<tinkoff::MarketDataResponse>
         readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    bool closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    void cancelMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    void finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;

    std::shared_ptr<PortfolioStream> createPortfolioStream(const QString& accountId) override;
    std::shared_ptr<PortfolioStream> createPortfolioStream(const QString& accountId, const QString& anotherAccountId) override;
    std::shared_ptr<tinkoff::PortfolioStreamResponse>
         readPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream) override;
    void cancelPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream) override;
    void finishPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream) override;

    std::shared_ptr<PositionsStream> createPositionsStream(const QString& accountId) override;
    std::shared_ptr<tinkoff::PositionsStreamResponse>
         readPositionsStream(std::shared_ptr<PositionsStream>& positionsStream) override;
    void cancelPositionsStream(std::shared_ptr<PositionsStream>& positionsStream) override;
    void finishPositionsStream(std::shared_ptr<PositionsStream>& positionsStream) override;

    void emitAuthFailed(const grpc::Status& status);

private:
    IRawGrpcClient*                                         mRawGrpcClient;
    ITimeUtils*                                             mTimeUtils;
    std::shared_ptr<grpc::CallCredentials>                  mCreds;
    std::unique_ptr<tinkoff::UsersService::Stub>            mUsersService;
    std::unique_ptr<tinkoff::InstrumentsService::Stub>      mInstrumentsService;
    std::unique_ptr<tinkoff::MarketDataService::Stub>       mMarketDataService;
    std::unique_ptr<tinkoff::MarketDataStreamService::Stub> mMarketDataStreamService;
    std::unique_ptr<tinkoff::OperationsService::Stub>       mOperationsService;
    std::unique_ptr<tinkoff::OperationsStreamService::Stub> mOperationsStreamService;
    std::unique_ptr<tinkoff::OrdersService::Stub>           mOrdersService;
};
