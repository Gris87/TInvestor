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
        getOperations,
        (const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
         grpc::ClientContext*                                     context,
         const tinkoff::OperationsRequest&                        req,
         tinkoff::OperationsResponse*                             resp),
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
};
