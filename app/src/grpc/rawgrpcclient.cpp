#include "src/grpc/rawgrpcclient.h"

#include <QDebug>



RawGrpcClient::RawGrpcClient() :
    IRawGrpcClient(),
    mMutex(new QMutex())
{
    qDebug() << "Create RawGrpcClient";
}

RawGrpcClient::~RawGrpcClient()
{
    qDebug() << "Destroy RawGrpcClient";

    delete mMutex;
}

MarketDataStream::Stream RawGrpcClient::createMarketDataStream(
    const std::unique_ptr<tinkoff::MarketDataStreamService::Stub>& service, grpc::ClientContext* context
)
{
    QMutexLocker lock(mMutex);

    return service->MarketDataStream(context);
}

bool
RawGrpcClient::writeMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, const tinkoff::MarketDataRequest& req)
{
    QMutexLocker lock(mMutex);

    return marketDataStream->stream->Write(req);
}

bool RawGrpcClient::readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, tinkoff::MarketDataResponse* resp)
{
    return marketDataStream->stream->Read(resp);
}

bool RawGrpcClient::closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    QMutexLocker lock(mMutex);

    return marketDataStream->stream->WritesDone();
}

grpc::Status RawGrpcClient::finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    QMutexLocker lock(mMutex);

    return marketDataStream->stream->Finish();
}
