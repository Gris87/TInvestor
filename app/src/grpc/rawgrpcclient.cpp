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

grpc::Status RawGrpcClient::getUserInfo(
    const std::unique_ptr<tinkoff::UsersService::Stub>& service,
    grpc::ClientContext*                                context,
    const tinkoff::GetInfoRequest&                      req,
    tinkoff::GetInfoResponse*                           resp
)
{
    QMutexLocker lock(mMutex);

    return service->GetInfo(context, req, resp);
}

grpc::Status RawGrpcClient::getAccounts(
    const std::unique_ptr<tinkoff::UsersService::Stub>& service,
    grpc::ClientContext*                                context,
    const tinkoff::GetAccountsRequest&                  req,
    tinkoff::GetAccountsResponse*                       resp
)
{
    QMutexLocker lock(mMutex);

    return service->GetAccounts(context, req, resp);
}

grpc::Status RawGrpcClient::findStocks(
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    tinkoff::SharesResponse*                                  resp
)
{
    QMutexLocker lock(mMutex);

    return service->Shares(context, req, resp);
}

grpc::Status RawGrpcClient::getCandles(
    const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::GetCandlesRequest&                        req,
    tinkoff::GetCandlesResponse*                             resp
)
{
    QMutexLocker lock(mMutex);

    return service->GetCandles(context, req, resp);
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
