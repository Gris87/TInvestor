#pragma once



#include "src/grpc/irawgrpcclient.h"

#include <gmock/gmock.h>



class RawGrpcClientMock : public IRawGrpcClient
{
public:
    explicit RawGrpcClientMock() :
        IRawGrpcClient()
    {
    }
    ~RawGrpcClientMock() override = default;

    RawGrpcClientMock(const RawGrpcClientMock& another)            = delete;
    RawGrpcClientMock& operator=(const RawGrpcClientMock& another) = delete;

    MOCK_METHOD(
        grpc::Status,
        getUserInfo,
        (const std::unique_ptr<tinkoff::UsersService::Stub>& service,
         grpc::ClientContext*                                context,
         const tinkoff::GetInfoRequest&                      req,
         tinkoff::GetInfoResponse*                           resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        getAccounts,
        (const std::unique_ptr<tinkoff::UsersService::Stub>& service,
         grpc::ClientContext*                                context,
         const tinkoff::GetAccountsRequest&                  req,
         tinkoff::GetAccountsResponse*                       resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        findStocks,
        (const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
         grpc::ClientContext*                                      context,
         const tinkoff::InstrumentsRequest&                        req,
         tinkoff::SharesResponse*                                  resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        findBonds,
        (const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
         grpc::ClientContext*                                      context,
         const tinkoff::InstrumentsRequest&                        req,
         tinkoff::BondsResponse*                                   resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        findCurrencies,
        (const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
         grpc::ClientContext*                                      context,
         const tinkoff::InstrumentsRequest&                        req,
         tinkoff::CurrenciesResponse*                              resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        findEtfs,
        (const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
         grpc::ClientContext*                                      context,
         const tinkoff::InstrumentsRequest&                        req,
         tinkoff::EtfsResponse*                                    resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        findFutures,
        (const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
         grpc::ClientContext*                                      context,
         const tinkoff::InstrumentsRequest&                        req,
         tinkoff::FuturesResponse*                                 resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        getCandles,
        (const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
         grpc::ClientContext*                                     context,
         const tinkoff::GetCandlesRequest&                        req,
         tinkoff::GetCandlesResponse*                             resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        getOrderBook,
        (const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
         grpc::ClientContext*                                     context,
         const tinkoff::GetOrderBookRequest&                      req,
         tinkoff::GetOrderBookResponse*                           resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        getPortfolio,
        (const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
         grpc::ClientContext*                                     context,
         const tinkoff::PortfolioRequest&                         req,
         tinkoff::PortfolioResponse*                              resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        getPositions,
        (const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
         grpc::ClientContext*                                     context,
         const tinkoff::PositionsRequest&                         req,
         tinkoff::PositionsResponse*                              resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        getOperations,
        (const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
         grpc::ClientContext*                                     context,
         const tinkoff::GetOperationsByCursorRequest&             req,
         tinkoff::GetOperationsByCursorResponse*                  resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        getMaxLots,
        (const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
         grpc::ClientContext*                                 context,
         const tinkoff::GetMaxLotsRequest&                    req,
         tinkoff::GetMaxLotsResponse*                         resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        postOrder,
        (const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
         grpc::ClientContext*                                 context,
         const tinkoff::PostOrderRequest&                     req,
         tinkoff::PostOrderResponse*                          resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        getOrderState,
        (const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
         grpc::ClientContext*                                 context,
         const tinkoff::GetOrderStateRequest&                 req,
         tinkoff::OrderState*                                 resp),
        (override)
    );
    MOCK_METHOD(
        grpc::Status,
        cancelOrder,
        (const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
         grpc::ClientContext*                                 context,
         const tinkoff::CancelOrderRequest&                   req,
         tinkoff::CancelOrderResponse*                        resp),
        (override)
    );

    MOCK_METHOD(
        MarketDataStream::Stream,
        createMarketDataStream,
        (const std::unique_ptr<tinkoff::MarketDataStreamService::Stub>& service, grpc::ClientContext* context),
        (override)
    );
    MOCK_METHOD(
        bool,
        writeMarketDataStream,
        (std::shared_ptr<MarketDataStream> & marketDataStream, const tinkoff::MarketDataRequest& req),
        (override)
    );
    MOCK_METHOD(
        bool,
        readMarketDataStream,
        (std::shared_ptr<MarketDataStream> & marketDataStream, tinkoff::MarketDataResponse* resp),
        (override)
    );
    MOCK_METHOD(bool, closeWriteMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));
    MOCK_METHOD(grpc::Status, finishMarketDataStream, (std::shared_ptr<MarketDataStream> & marketDataStream), (override));

    MOCK_METHOD(
        PortfolioStream::Stream,
        createPortfolioStream,
        (const std::unique_ptr<tinkoff::OperationsStreamService::Stub>& service,
         grpc::ClientContext*                                           context,
         const tinkoff::PortfolioStreamRequest&                         req),
        (override)
    );
    MOCK_METHOD(
        bool,
        readPortfolioStream,
        (std::shared_ptr<PortfolioStream> & portfolioStream, tinkoff::PortfolioStreamResponse* resp),
        (override)
    );
    MOCK_METHOD(grpc::Status, finishPortfolioStream, (std::shared_ptr<PortfolioStream> & portfolioStream), (override));

    MOCK_METHOD(
        PositionsStream::Stream,
        createPositionsStream,
        (const std::unique_ptr<tinkoff::OperationsStreamService::Stub>& service,
         grpc::ClientContext*                                           context,
         const tinkoff::PositionsStreamRequest&                         req),
        (override)
    );
    MOCK_METHOD(
        bool,
        readPositionsStream,
        (std::shared_ptr<PositionsStream> & positionsStream, tinkoff::PositionsStreamResponse* resp),
        (override)
    );
    MOCK_METHOD(grpc::Status, finishPositionsStream, (std::shared_ptr<PositionsStream> & positionsStream), (override));
};
