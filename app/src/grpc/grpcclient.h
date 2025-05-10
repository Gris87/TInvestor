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

                if (status.error_code() != grpc::StatusCode::CANCELLED)
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
    std::shared_ptr<tinkoff::SharesResponse>      findStocks(QThread* parentThread) override;
    std::shared_ptr<tinkoff::BondsResponse>       findBonds(QThread* parentThread) override;
    std::shared_ptr<tinkoff::CurrenciesResponse>  findCurrencies(QThread* parentThread) override;
    std::shared_ptr<tinkoff::EtfsResponse>        findEtfs(QThread* parentThread) override;
    std::shared_ptr<tinkoff::FuturesResponse>     findFutures(QThread* parentThread) override;
    std::shared_ptr<tinkoff::GetCandlesResponse>
    getCandles(QThread* parentThread, const QString& uid, qint64 from, qint64 to) override;
    std::shared_ptr<tinkoff::GetOrderBookResponse> getOrderBook(QThread* parentThread, const QString& uid) override;
    std::shared_ptr<tinkoff::PortfolioResponse>    getPortfolio(QThread* parentThread, const QString& accountId) override;
    std::shared_ptr<tinkoff::OperationsResponse>   getOperations(QThread* parentThread, const QString& accountId) override;

    std::shared_ptr<MarketDataStream> createMarketDataStream() override;
    bool subscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream, const QStringList& uids) override;
    bool unsubscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    bool subscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream, const QString& uid) override;
    bool unsubscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    std::shared_ptr<tinkoff::MarketDataResponse>
         readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    bool closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    void finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;

    std::shared_ptr<PortfolioStream> createPortfolioStream(const QString& accountId) override;
    std::shared_ptr<tinkoff::PortfolioStreamResponse>
         readPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream) override;
    void finishPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream) override;

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
};
