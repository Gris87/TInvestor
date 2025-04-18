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
    const QMutexLocker lock(mMutex);

    return service->GetInfo(context, req, resp);
}

grpc::Status RawGrpcClient::getAccounts(
    const std::unique_ptr<tinkoff::UsersService::Stub>& service,
    grpc::ClientContext*                                context,
    const tinkoff::GetAccountsRequest&                  req,
    tinkoff::GetAccountsResponse*                       resp
)
{
    const QMutexLocker lock(mMutex);

    return service->GetAccounts(context, req, resp);
}

grpc::Status RawGrpcClient::findStocks(
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    tinkoff::SharesResponse*                                  resp
)
{
    const QMutexLocker lock(mMutex);

    return service->Shares(context, req, resp);
}

grpc::Status RawGrpcClient::getCandles(
    const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::GetCandlesRequest&                        req,
    tinkoff::GetCandlesResponse*                             resp
)
{
    const QMutexLocker lock(mMutex);

    return service->GetCandles(context, req, resp);
}

grpc::Status RawGrpcClient::getOrderBook(
    const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::GetOrderBookRequest&                      req,
    tinkoff::GetOrderBookResponse*                           resp
)
{
    const QMutexLocker lock(mMutex);

    return service->GetOrderBook(context, req, resp);
}

MarketDataStream::Stream RawGrpcClient::createMarketDataStream(
    const std::unique_ptr<tinkoff::MarketDataStreamService::Stub>& service, grpc::ClientContext* context
)
{
    const QMutexLocker lock(mMutex);

    return service->MarketDataStream(context);
}

bool
RawGrpcClient::writeMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, const tinkoff::MarketDataRequest& req)
{
    const QMutexLocker lock(mMutex);

    return marketDataStream->stream->Write(req);
}

bool RawGrpcClient::readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream, tinkoff::MarketDataResponse* resp)
{
    return marketDataStream->stream->Read(resp);
}

bool RawGrpcClient::closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    const QMutexLocker lock(mMutex);

    return marketDataStream->stream->WritesDone();
}

grpc::Status RawGrpcClient::finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    const QMutexLocker lock(mMutex);

    return marketDataStream->stream->Finish();
}
