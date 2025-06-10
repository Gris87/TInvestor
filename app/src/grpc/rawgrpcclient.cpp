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

grpc::Status RawGrpcClient::findBonds(
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    tinkoff::BondsResponse*                                   resp
)
{
    const QMutexLocker lock(mMutex);

    return service->Bonds(context, req, resp);
}

grpc::Status RawGrpcClient::findCurrencies(
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    tinkoff::CurrenciesResponse*                              resp
)
{
    const QMutexLocker lock(mMutex);

    return service->Currencies(context, req, resp);
}

grpc::Status RawGrpcClient::findEtfs(
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    tinkoff::EtfsResponse*                                    resp
)
{
    const QMutexLocker lock(mMutex);

    return service->Etfs(context, req, resp);
}

grpc::Status RawGrpcClient::findFutures(
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    tinkoff::FuturesResponse*                                 resp
)
{
    const QMutexLocker lock(mMutex);

    return service->Futures(context, req, resp);
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

grpc::Status RawGrpcClient::getPortfolio(
    const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::PortfolioRequest&                         req,
    tinkoff::PortfolioResponse*                              resp
)
{
    const QMutexLocker lock(mMutex);

    return service->GetPortfolio(context, req, resp);
}

grpc::Status RawGrpcClient::getPositions(
    const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::PositionsRequest&                         req,
    tinkoff::PositionsResponse*                              resp
)
{
    const QMutexLocker lock(mMutex);

    return service->GetPositions(context, req, resp);
}

grpc::Status RawGrpcClient::getOperations(
    const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::GetOperationsByCursorRequest&             req,
    tinkoff::GetOperationsByCursorResponse*                  resp
)
{
    const QMutexLocker lock(mMutex);

    return service->GetOperationsByCursor(context, req, resp);
}

grpc::Status RawGrpcClient::getMaxLots(
    const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
    grpc::ClientContext*                                 context,
    const tinkoff::GetMaxLotsRequest&                    req,
    tinkoff::GetMaxLotsResponse*                         resp
)
{
    const QMutexLocker lock(mMutex);

    return service->GetMaxLots(context, req, resp);
}

grpc::Status RawGrpcClient::postOrder(
    const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
    grpc::ClientContext*                                 context,
    const tinkoff::PostOrderRequest&                     req,
    tinkoff::PostOrderResponse*                          resp
)
{
    const QMutexLocker lock(mMutex);

    return service->PostOrder(context, req, resp);
}

grpc::Status RawGrpcClient::getOrderState(
    const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
    grpc::ClientContext*                                 context,
    const tinkoff::GetOrderStateRequest&                 req,
    tinkoff::OrderState*                                 resp
)
{
    const QMutexLocker lock(mMutex);

    return service->GetOrderState(context, req, resp);
}

grpc::Status RawGrpcClient::cancelOrder(
    const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
    grpc::ClientContext*                                 context,
    const tinkoff::CancelOrderRequest&                   req,
    tinkoff::CancelOrderResponse*                        resp
)
{
    const QMutexLocker lock(mMutex);

    return service->CancelOrder(context, req, resp);
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

PortfolioStream::Stream RawGrpcClient::createPortfolioStream(
    const std::unique_ptr<tinkoff::OperationsStreamService::Stub>& service,
    grpc::ClientContext*                                           context,
    const tinkoff::PortfolioStreamRequest&                         req
)
{
    const QMutexLocker lock(mMutex);

    return service->PortfolioStream(context, req);
}

bool RawGrpcClient::readPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream, tinkoff::PortfolioStreamResponse* resp)
{
    return portfolioStream->stream->Read(resp);
}

grpc::Status RawGrpcClient::finishPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream)
{
    const QMutexLocker lock(mMutex);

    return portfolioStream->stream->Finish();
}

PositionsStream::Stream RawGrpcClient::createPositionsStream(
    const std::unique_ptr<tinkoff::OperationsStreamService::Stub>& service,
    grpc::ClientContext*                                           context,
    const tinkoff::PositionsStreamRequest&                         req
)
{
    const QMutexLocker lock(mMutex);

    return service->PositionsStream(context, req);
}

bool RawGrpcClient::readPositionsStream(std::shared_ptr<PositionsStream>& positionsStream, tinkoff::PositionsStreamResponse* resp)
{
    return positionsStream->stream->Read(resp);
}

grpc::Status RawGrpcClient::finishPositionsStream(std::shared_ptr<PositionsStream>& positionsStream)
{
    const QMutexLocker lock(mMutex);

    return positionsStream->stream->Finish();
}
