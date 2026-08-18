#pragma once



#include <QObject>



#ifdef Q_OS_WINDOWS
#pragma warning(push)
#pragma warning(disable : 4018 4100 4267 4996)
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-requires"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include "messages/generated/instruments.grpc.pb.h"
#include "messages/generated/marketdata.grpc.pb.h"
#include "messages/generated/operations.grpc.pb.h"
#include "messages/generated/orders.grpc.pb.h"
#include "messages/generated/users.grpc.pb.h"

#ifdef Q_OS_WINDOWS
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
    using Stream = std::unique_ptr<grpc::ClientReader<tinkoff::MarketDataResponse>>;

    grpc::ClientContext context;
    Stream              stream;
};

struct OperationsStream
{
    using Stream = std::unique_ptr<grpc::ClientReader<tinkoff::OperationsStreamResponse>>;

    grpc::ClientContext context;
    Stream              stream;
};

struct PortfolioStream
{
    using Stream = std::unique_ptr<grpc::ClientReader<tinkoff::PortfolioStreamResponse>>;

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
    virtual grpc::Status getPortfolio(
        const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
        grpc::ClientContext*                                     context,
        const tinkoff::PortfolioRequest&                         req,
        tinkoff::PortfolioResponse*                              resp
    ) = 0;
    virtual grpc::Status getOperations(
        const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
        grpc::ClientContext*                                     context,
        const tinkoff::GetOperationsByCursorRequest&             req,
        tinkoff::GetOperationsByCursorResponse*                  resp
    ) = 0;
    virtual grpc::Status getMaxLots(
        const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
        grpc::ClientContext*                                 context,
        const tinkoff::GetMaxLotsRequest&                    req,
        tinkoff::GetMaxLotsResponse*                         resp
    ) = 0;
    virtual grpc::Status postOrder(
        const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
        grpc::ClientContext*                                 context,
        const tinkoff::PostOrderRequest&                     req,
        tinkoff::PostOrderResponse*                          resp
    ) = 0;
    virtual grpc::Status getOrderState(
        const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
        grpc::ClientContext*                                 context,
        const tinkoff::GetOrderStateRequest&                 req,
        tinkoff::OrderState*                                 resp
    ) = 0;
    virtual grpc::Status cancelOrder(
        const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
        grpc::ClientContext*                                 context,
        const tinkoff::CancelOrderRequest&                   req,
        tinkoff::CancelOrderResponse*                        resp
    ) = 0;

    virtual MarketDataStream::Stream createMarketDataStream(
        const std::unique_ptr<tinkoff::MarketDataStreamService::Stub>& service,
        grpc::ClientContext*                                           context,
        const tinkoff::MarketDataServerSideStreamRequest&              req
    )                                                                                                                         = 0;
    virtual bool readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, tinkoff::MarketDataResponse* resp) = 0;
    virtual grpc::Status finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)                          = 0;

    virtual OperationsStream::Stream createOperationsStream(
        const std::unique_ptr<tinkoff::OperationsStreamService::Stub>& service,
        grpc::ClientContext*                                           context,
        const tinkoff::OperationsStreamRequest&                        req
    ) = 0;
    virtual bool
    readOperationsStream(std::shared_ptr<OperationsStream>& operationsStream, tinkoff::OperationsStreamResponse* resp) = 0;
    virtual grpc::Status finishOperationsStream(std::shared_ptr<OperationsStream>& operationsStream)                   = 0;

    virtual PortfolioStream::Stream createPortfolioStream(
        const std::unique_ptr<tinkoff::OperationsStreamService::Stub>& service,
        grpc::ClientContext*                                           context,
        const tinkoff::PortfolioStreamRequest&                         req
    ) = 0;
    virtual bool
    readPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream, tinkoff::PortfolioStreamResponse* resp) = 0;
    virtual grpc::Status finishPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream)                  = 0;
};
