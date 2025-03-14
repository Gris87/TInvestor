#pragma once



#include "src/grpc/irawgrpcclient.h"

#include <QMutex>



class RawGrpcClient : public IRawGrpcClient
{
public:
    explicit RawGrpcClient();
    ~RawGrpcClient();

    RawGrpcClient(const RawGrpcClient& another)            = delete;
    RawGrpcClient& operator=(const RawGrpcClient& another) = delete;

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
