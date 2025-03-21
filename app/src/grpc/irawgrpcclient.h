#pragma once



#include <QObject>



#pragma warning(push)
#pragma warning(disable : 4100 4189 4267)
#include "messages/generated/instruments.grpc.pb.h"
#include "messages/generated/marketdata.grpc.pb.h"
#include "messages/generated/users.grpc.pb.h"
#pragma warning(pop)



#ifndef USE_SANDBOX
#define ADDRESS "invest-public-api.tinkoff.ru:443"
#else
#define ADDRESS "sandbox-invest-public-api.tinkoff.ru:443"
#endif

#define MAX_LIMIT_FOR_INTERVAL_1_MIN 2400



namespace tinkoff
{
using namespace tinkoff::public_::invest::api::contract::v1;
}



struct MarketDataStream
{
    typedef std::unique_ptr<grpc::ClientReaderWriter<tinkoff::MarketDataRequest, tinkoff::MarketDataResponse>> Stream;

    grpc::ClientContext context;
    Stream              stream;
};



class IRawGrpcClient
{
public:
    explicit IRawGrpcClient()
    {
    }
    virtual ~IRawGrpcClient() = default;

    IRawGrpcClient(const IRawGrpcClient& another)            = delete;
    IRawGrpcClient& operator=(const IRawGrpcClient& another) = delete;

    virtual grpc::Status getUserInfo(
        const std::unique_ptr<tinkoff::UsersService::Stub>& service,
        grpc::ClientContext*                                context,
        const tinkoff::GetInfoRequest&                      req,
        tinkoff::GetInfoResponse*                           resp
    ) = 0;
    virtual grpc::Status getAccounts(
        const std::unique_ptr<tinkoff::UsersService::Stub>& service,
        grpc::ClientContext*                                context,
        const tinkoff::GetAccountsRequest&                  req,
        tinkoff::GetAccountsResponse*                       resp
    ) = 0;
    virtual grpc::Status findStocks(
        const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
        grpc::ClientContext*                                      context,
        const tinkoff::InstrumentsRequest&                        req,
        tinkoff::SharesResponse*                                  resp
    ) = 0;
    virtual grpc::Status getCandles(
        const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
        grpc::ClientContext*                                     context,
        const tinkoff::GetCandlesRequest&                        req,
        tinkoff::GetCandlesResponse*                             resp
    ) = 0;
    virtual grpc::Status getOrderBook(
        const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
        grpc::ClientContext*                                     context,
        const tinkoff::GetOrderBookRequest&                      req,
        tinkoff::GetOrderBookResponse*                           resp
    ) = 0;

    virtual MarketDataStream::Stream createMarketDataStream(
        const std::unique_ptr<tinkoff::MarketDataStreamService::Stub>& service, grpc::ClientContext* context
    ) = 0;
    virtual bool
    writeMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, const tinkoff::MarketDataRequest& req)         = 0;
    virtual bool readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, tinkoff::MarketDataResponse* resp) = 0;
    virtual bool closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)                              = 0;
    virtual grpc::Status finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)                          = 0;
};
