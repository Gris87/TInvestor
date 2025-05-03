#pragma once



#include <QObject>



#if defined(Q_OS_WINDOWS)
#pragma warning(push)
#pragma warning(disable : 4100 4189 4267)
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-requires"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "messages/generated/instruments.grpc.pb.h"
#include "messages/generated/marketdata.grpc.pb.h"
#include "messages/generated/users.grpc.pb.h"

#if defined(Q_OS_WINDOWS)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif



// NOLINTBEGIN(google-build-using-namespace)
namespace tinkoff
{
using namespace tinkoff::public_::invest::api::contract::v1;
}
// NOLINTEND(google-build-using-namespace)



struct MarketDataStream
{
    using Stream = std::unique_ptr<grpc::ClientReaderWriter<tinkoff::MarketDataRequest, tinkoff::MarketDataResponse>>;

    grpc::ClientContext context;
    Stream              stream;
};



class IRawGrpcClient
{
public:
    explicit IRawGrpcClient() = default;
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
    virtual grpc::Status findBonds(
        const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
        grpc::ClientContext*                                      context,
        const tinkoff::InstrumentsRequest&                        req,
        tinkoff::BondsResponse*                                   resp
    ) = 0;
    virtual grpc::Status findCurrencies(
        const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
        grpc::ClientContext*                                      context,
        const tinkoff::InstrumentsRequest&                        req,
        tinkoff::CurrenciesResponse*                              resp
    ) = 0;
    virtual grpc::Status findEtfs(
        const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
        grpc::ClientContext*                                      context,
        const tinkoff::InstrumentsRequest&                        req,
        tinkoff::EtfsResponse*                                    resp
    ) = 0;
    virtual grpc::Status findFutures(
        const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
        grpc::ClientContext*                                      context,
        const tinkoff::InstrumentsRequest&                        req,
        tinkoff::FuturesResponse*                                 resp
    ) = 0;
    virtual grpc::Status findOptions(
        const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
        grpc::ClientContext*                                      context,
        const tinkoff::InstrumentsRequest&                        req,
        tinkoff::OptionsResponse*                                 resp
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
