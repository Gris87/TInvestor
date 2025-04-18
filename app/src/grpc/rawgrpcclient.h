#pragma once



#include "src/grpc/irawgrpcclient.h"

#include <QMutex>



class RawGrpcClient : public IRawGrpcClient
{
public:
    explicit RawGrpcClient();
    ~RawGrpcClient() override;

    RawGrpcClient(const RawGrpcClient& another)            = delete;
    RawGrpcClient& operator=(const RawGrpcClient& another) = delete;

    grpc::Status getUserInfo(
        const std::unique_ptr<tinkoff::UsersService::Stub>& service,
        grpc::ClientContext*                                context,
        const tinkoff::GetInfoRequest&                      req,
        tinkoff::GetInfoResponse*                           resp
    ) override;
    grpc::Status getAccounts(
        const std::unique_ptr<tinkoff::UsersService::Stub>& service,
        grpc::ClientContext*                                context,
        const tinkoff::GetAccountsRequest&                  req,
        tinkoff::GetAccountsResponse*                       resp
    ) override;
    grpc::Status findStocks(
        const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
        grpc::ClientContext*                                      context,
        const tinkoff::InstrumentsRequest&                        req,
        tinkoff::SharesResponse*                                  resp
    ) override;
    grpc::Status getCandles(
        const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
        grpc::ClientContext*                                     context,
        const tinkoff::GetCandlesRequest&                        req,
        tinkoff::GetCandlesResponse*                             resp
    ) override;
    grpc::Status getOrderBook(
        const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
        grpc::ClientContext*                                     context,
        const tinkoff::GetOrderBookRequest&                      req,
        tinkoff::GetOrderBookResponse*                           resp
    ) override;

    MarketDataStream::Stream createMarketDataStream(
        const std::unique_ptr<tinkoff::MarketDataStreamService::Stub>& service, grpc::ClientContext* context
    ) override;
    bool
    writeMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, const tinkoff::MarketDataRequest& req) override;
    bool readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, tinkoff::MarketDataResponse* resp) override;
    bool closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;
    grpc::Status finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream) override;

private:
    QMutex* mMutex;
};
