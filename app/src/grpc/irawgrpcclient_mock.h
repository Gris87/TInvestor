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
