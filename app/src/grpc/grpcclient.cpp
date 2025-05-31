#include "src/grpc/grpcclient.h"

#include <QDebug>
#include <grpcpp/grpcpp.h>
#include <memory>

#include "src/grpc/investapiuthenticator.h"



#ifndef USE_SANDBOX
const char* const GRPC_ADDRESS = "invest-public-api.tinkoff.ru:443";
#else
const char* const GRPC_ADDRESS = "sandbox-invest-public-api.tinkoff.ru:443";
#endif

constexpr int    MAX_LIMIT_FOR_INTERVAL_1_MIN = 2400;
constexpr int    ORDER_BOOK_DEPTH             = 50;
constexpr int    OPERATIONS_LIMIT             = 1000;
constexpr qint64 MS_IN_SECOND                 = 1000LL;



// clang-format off
static const QMap<grpc::StatusCode, QString> GRPC_STATUS_CODE_TO_STRING{ // clazy:exclude=non-pod-global-static
    {grpc::StatusCode::OK,                  "OK"                 },
    {grpc::StatusCode::CANCELLED,           "CANCELLED"          },
    {grpc::StatusCode::UNKNOWN,             "UNKNOWN"            },
    {grpc::StatusCode::INVALID_ARGUMENT,    "INVALID_ARGUMENT"   },
    {grpc::StatusCode::DEADLINE_EXCEEDED,   "DEADLINE_EXCEEDED"  },
    {grpc::StatusCode::NOT_FOUND,           "NOT_FOUND"          },
    {grpc::StatusCode::ALREADY_EXISTS,      "ALREADY_EXISTS"     },
    {grpc::StatusCode::PERMISSION_DENIED,   "PERMISSION_DENIED"  },
    {grpc::StatusCode::UNAUTHENTICATED,     "UNAUTHENTICATED"    },
    {grpc::StatusCode::RESOURCE_EXHAUSTED,  "RESOURCE_EXHAUSTED" },
    {grpc::StatusCode::FAILED_PRECONDITION, "FAILED_PRECONDITION"},
    {grpc::StatusCode::ABORTED,             "ABORTED"            },
    {grpc::StatusCode::OUT_OF_RANGE,        "OUT_OF_RANGE"       },
    {grpc::StatusCode::UNIMPLEMENTED,       "UNIMPLEMENTED"      },
    {grpc::StatusCode::INTERNAL,            "INTERNAL"           },
    {grpc::StatusCode::UNAVAILABLE,         "UNAVAILABLE"        },
    {grpc::StatusCode::DATA_LOSS,           "DATA_LOSS"          },
    {grpc::StatusCode::DO_NOT_USE,          "DO_NOT_USE"         },
};
// clang-format on



GrpcClient::GrpcClient(IUserStorage* userStorage, IRawGrpcClient* rawGrpcClient, ITimeUtils* timeUtils, QObject* parent) :
    IGrpcClient(parent),
    mRawGrpcClient(rawGrpcClient),
    mTimeUtils(timeUtils)
{
    qDebug() << "Create GrpcClient";

    mCreds = grpc::MetadataCredentialsFromPlugin(
        std::unique_ptr<grpc::MetadataCredentialsPlugin>(new InvestApiAuthenticator(userStorage))
    );

    const std::shared_ptr<grpc::Channel> channel =
        grpc::CreateChannel(GRPC_ADDRESS, grpc::SslCredentials(grpc::SslCredentialsOptions()));

    mUsersService            = tinkoff::UsersService::NewStub(channel);
    mInstrumentsService      = tinkoff::InstrumentsService::NewStub(channel);
    mMarketDataService       = tinkoff::MarketDataService::NewStub(channel);
    mMarketDataStreamService = tinkoff::MarketDataStreamService::NewStub(channel);
    mOperationsService       = tinkoff::OperationsService::NewStub(channel);
    mOperationsStreamService = tinkoff::OperationsStreamService::NewStub(channel);
}

GrpcClient::~GrpcClient()
{
    qDebug() << "Destroy GrpcClient";
}

static grpc::Status getUserInfoAction(
    IRawGrpcClient*                                     rawGrpcClient,
    const std::unique_ptr<tinkoff::UsersService::Stub>& service,
    grpc::ClientContext*                                context,
    const tinkoff::GetInfoRequest&                      req,
    const std::shared_ptr<tinkoff::GetInfoResponse>&    resp
)
{
    return rawGrpcClient->getUserInfo(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::GetInfoResponse> GrpcClient::getUserInfo(QThread* parentThread)
{
    grpc::ClientContext                             context;
    const tinkoff::GetInfoRequest                   req;
    const std::shared_ptr<tinkoff::GetInfoResponse> resp = std::make_shared<tinkoff::GetInfoResponse>();

    context.set_credentials(mCreds);

    return repeatRequest(parentThread, getUserInfoAction, mUsersService, &context, req, resp);
}

static grpc::Status getAccountsAction(
    IRawGrpcClient*                                      rawGrpcClient,
    const std::unique_ptr<tinkoff::UsersService::Stub>&  service,
    grpc::ClientContext*                                 context,
    const tinkoff::GetAccountsRequest&                   req,
    const std::shared_ptr<tinkoff::GetAccountsResponse>& resp
)
{
    return rawGrpcClient->getAccounts(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::GetAccountsResponse> GrpcClient::getAccounts(QThread* parentThread)
{
    grpc::ClientContext                                 context;
    tinkoff::GetAccountsRequest                         req;
    const std::shared_ptr<tinkoff::GetAccountsResponse> resp = std::make_shared<tinkoff::GetAccountsResponse>();

    context.set_credentials(mCreds);

    req.set_status(tinkoff::ACCOUNT_STATUS_OPEN);

    return repeatRequest(parentThread, getAccountsAction, mUsersService, &context, req, resp);
}

static grpc::Status findStocksAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::SharesResponse>&           resp
)
{
    return rawGrpcClient->findStocks(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::SharesResponse> GrpcClient::findStocks(QThread* parentThread, tinkoff::InstrumentStatus instrumentStatus)
{
    grpc::ClientContext                            context;
    tinkoff::InstrumentsRequest                    req;
    const std::shared_ptr<tinkoff::SharesResponse> resp = std::make_shared<tinkoff::SharesResponse>();

    context.set_credentials(mCreds);

    req.set_instrument_status(instrumentStatus);

    return repeatRequest(parentThread, findStocksAction, mInstrumentsService, &context, req, resp);
}

static grpc::Status findBondsAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::BondsResponse>&            resp
)
{
    return rawGrpcClient->findBonds(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::BondsResponse> GrpcClient::findBonds(QThread* parentThread)
{
    grpc::ClientContext                           context;
    tinkoff::InstrumentsRequest                   req;
    const std::shared_ptr<tinkoff::BondsResponse> resp = std::make_shared<tinkoff::BondsResponse>();

    context.set_credentials(mCreds);

    req.set_instrument_status(tinkoff::INSTRUMENT_STATUS_ALL);

    return repeatRequest(parentThread, findBondsAction, mInstrumentsService, &context, req, resp);
}

static grpc::Status findCurrenciesAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::CurrenciesResponse>&       resp
)
{
    return rawGrpcClient->findCurrencies(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::CurrenciesResponse> GrpcClient::findCurrencies(QThread* parentThread)
{
    grpc::ClientContext                                context;
    tinkoff::InstrumentsRequest                        req;
    const std::shared_ptr<tinkoff::CurrenciesResponse> resp = std::make_shared<tinkoff::CurrenciesResponse>();

    context.set_credentials(mCreds);

    req.set_instrument_status(tinkoff::INSTRUMENT_STATUS_ALL);

    return repeatRequest(parentThread, findCurrenciesAction, mInstrumentsService, &context, req, resp);
}

static grpc::Status findEtfsAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::EtfsResponse>&             resp
)
{
    return rawGrpcClient->findEtfs(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::EtfsResponse> GrpcClient::findEtfs(QThread* parentThread)
{
    grpc::ClientContext                          context;
    tinkoff::InstrumentsRequest                  req;
    const std::shared_ptr<tinkoff::EtfsResponse> resp = std::make_shared<tinkoff::EtfsResponse>();

    context.set_credentials(mCreds);

    req.set_instrument_status(tinkoff::INSTRUMENT_STATUS_ALL);

    return repeatRequest(parentThread, findEtfsAction, mInstrumentsService, &context, req, resp);
}

static grpc::Status findFuturesAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    grpc::ClientContext*                                      context,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::FuturesResponse>&          resp
)
{
    return rawGrpcClient->findFutures(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::FuturesResponse> GrpcClient::findFutures(QThread* parentThread)
{
    grpc::ClientContext                             context;
    tinkoff::InstrumentsRequest                     req;
    const std::shared_ptr<tinkoff::FuturesResponse> resp = std::make_shared<tinkoff::FuturesResponse>();

    context.set_credentials(mCreds);

    req.set_instrument_status(tinkoff::INSTRUMENT_STATUS_ALL);

    return repeatRequest(parentThread, findFuturesAction, mInstrumentsService, &context, req, resp);
}

static grpc::Status getCandlesAction(
    IRawGrpcClient*                                          rawGrpcClient,
    const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::GetCandlesRequest&                        req,
    const std::shared_ptr<tinkoff::GetCandlesResponse>&      resp
)
{
    return rawGrpcClient->getCandles(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::GetCandlesResponse>
GrpcClient::getCandles(QThread* parentThread, const QString& instrumentId, qint64 from, qint64 to)
{
    grpc::ClientContext                                context;
    tinkoff::GetCandlesRequest                         req;
    const std::shared_ptr<tinkoff::GetCandlesResponse> resp = std::make_shared<tinkoff::GetCandlesResponse>();

    context.set_credentials(mCreds);

    google::protobuf::Timestamp* fromTimestamp = new google::protobuf::Timestamp(); // req will take ownership
    google::protobuf::Timestamp* toTimestamp   = new google::protobuf::Timestamp(); // req will take ownership

    fromTimestamp->set_seconds(from / MS_IN_SECOND);
    fromTimestamp->set_nanos(0);
    toTimestamp->set_seconds(to / MS_IN_SECOND);
    toTimestamp->set_nanos(0);

    req.set_instrument_id(instrumentId.toStdString());
    req.set_allocated_from(fromTimestamp);
    req.set_allocated_to(toTimestamp);
    req.set_interval(tinkoff::CANDLE_INTERVAL_1_MIN);
    req.set_limit(MAX_LIMIT_FOR_INTERVAL_1_MIN);

    return repeatRequest(parentThread, getCandlesAction, mMarketDataService, &context, req, resp);
}

static grpc::Status getOrderBookAction(
    IRawGrpcClient*                                          rawGrpcClient,
    const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::GetOrderBookRequest&                      req,
    const std::shared_ptr<tinkoff::GetOrderBookResponse>&    resp
)
{
    return rawGrpcClient->getOrderBook(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::GetOrderBookResponse> GrpcClient::getOrderBook(QThread* parentThread, const QString& instrumentId)
{
    grpc::ClientContext                                  context;
    tinkoff::GetOrderBookRequest                         req;
    const std::shared_ptr<tinkoff::GetOrderBookResponse> resp = std::make_shared<tinkoff::GetOrderBookResponse>();

    context.set_credentials(mCreds);

    req.set_instrument_id(instrumentId.toStdString());
    req.set_depth(ORDER_BOOK_DEPTH);

    return repeatRequest(parentThread, getOrderBookAction, mMarketDataService, &context, req, resp);
}

static grpc::Status getPortfolioAction(
    IRawGrpcClient*                                          rawGrpcClient,
    const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
    grpc::ClientContext*                                     context,
    const tinkoff::PortfolioRequest&                         req,
    const std::shared_ptr<tinkoff::PortfolioResponse>&       resp
)
{
    return rawGrpcClient->getPortfolio(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::PortfolioResponse> GrpcClient::getPortfolio(QThread* parentThread, const QString& accountId)
{
    grpc::ClientContext                               context;
    tinkoff::PortfolioRequest                         req;
    const std::shared_ptr<tinkoff::PortfolioResponse> resp = std::make_shared<tinkoff::PortfolioResponse>();

    context.set_credentials(mCreds);

    req.set_account_id(accountId.toStdString());

    return repeatRequest(parentThread, getPortfolioAction, mOperationsService, &context, req, resp);
}

static grpc::Status getOperationsAction(
    IRawGrpcClient*                                                rawGrpcClient,
    const std::unique_ptr<tinkoff::OperationsService::Stub>&       service,
    grpc::ClientContext*                                           context,
    const tinkoff::GetOperationsByCursorRequest&                   req,
    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse>& resp
)
{
    return rawGrpcClient->getOperations(service, context, req, resp.get());
}

std::shared_ptr<tinkoff::GetOperationsByCursorResponse>
GrpcClient::getOperations(QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor)
{
    grpc::ClientContext                                           context;
    tinkoff::GetOperationsByCursorRequest                         req;
    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> resp =
        std::make_shared<tinkoff::GetOperationsByCursorResponse>();

    context.set_credentials(mCreds);

    google::protobuf::Timestamp* fromTimestamp = new google::protobuf::Timestamp(); // req will take ownership
    google::protobuf::Timestamp* toTimestamp   = new google::protobuf::Timestamp(); // req will take ownership

    fromTimestamp->set_seconds(from / MS_IN_SECOND);
    fromTimestamp->set_nanos(0);
    toTimestamp->set_seconds(to / MS_IN_SECOND);
    toTimestamp->set_nanos(0);

    req.set_account_id(accountId.toStdString());
    req.set_allocated_from(fromTimestamp);
    req.set_allocated_to(toTimestamp);
    req.set_cursor(cursor.toStdString());
    req.set_limit(OPERATIONS_LIMIT);
    req.set_state(tinkoff::OPERATION_STATE_EXECUTED);
    req.set_without_trades(true);

    return repeatRequest(parentThread, getOperationsAction, mOperationsService, &context, req, resp);
}

std::shared_ptr<MarketDataStream> GrpcClient::createMarketDataStream()
{
    std::shared_ptr<MarketDataStream> res = std::make_shared<MarketDataStream>();

    res->context.set_credentials(mCreds);
    res->stream = mRawGrpcClient->createMarketDataStream(mMarketDataStreamService, &res->context);

    return res;
}

bool GrpcClient::subscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream, const QStringList& instrumentIds)
{
    tinkoff::MarketDataRequest          req;
    tinkoff::SubscribeLastPriceRequest* subscribeLastPriceRequest =
        new tinkoff::SubscribeLastPriceRequest(); // req will take ownership

    subscribeLastPriceRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_SUBSCRIBE);

    for (const QString& instrumentId : instrumentIds)
    {
        subscribeLastPriceRequest->add_instruments()->set_instrument_id(instrumentId.toStdString());
    }

    req.set_allocated_subscribe_last_price_request(subscribeLastPriceRequest);

    const bool res = mRawGrpcClient->writeMarketDataStream(marketDataStream, req);

    if (!res)
    {
        emit authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "GrpcClient::subscribeLastPrices()");
    }

    return res;
}

bool GrpcClient::unsubscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    tinkoff::MarketDataRequest          req;
    tinkoff::SubscribeLastPriceRequest* subscribeLastPriceRequest =
        new tinkoff::SubscribeLastPriceRequest(); // req will take ownership

    subscribeLastPriceRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_UNSUBSCRIBE);

    req.set_allocated_subscribe_last_price_request(subscribeLastPriceRequest);

    const bool res = mRawGrpcClient->writeMarketDataStream(marketDataStream, req);

    if (!res)
    {
        emit authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "GrpcClient::unsubscribeLastPrices()");
    }

    return res;
}

bool GrpcClient::subscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream, const QString& instrumentId)
{
    tinkoff::MarketDataRequest          req;
    tinkoff::SubscribeOrderBookRequest* subscribeOrderBookRequest =
        new tinkoff::SubscribeOrderBookRequest(); // req will take ownership

    subscribeOrderBookRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_SUBSCRIBE);
    tinkoff::OrderBookInstrument* orderBook = subscribeOrderBookRequest->add_instruments();

    orderBook->set_instrument_id(instrumentId.toStdString());
    orderBook->set_depth(ORDER_BOOK_DEPTH);
    orderBook->set_order_book_type(tinkoff::ORDERBOOK_TYPE_ALL);

    req.set_allocated_subscribe_order_book_request(subscribeOrderBookRequest);

    const bool res = mRawGrpcClient->writeMarketDataStream(marketDataStream, req);

    if (!res)
    {
        emit authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "GrpcClient::subscribeOrderBook()");
    }

    return res;
}

bool GrpcClient::unsubscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    tinkoff::MarketDataRequest          req;
    tinkoff::SubscribeOrderBookRequest* subscribeOrderBookRequest =
        new tinkoff::SubscribeOrderBookRequest(); // req will take ownership

    subscribeOrderBookRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_UNSUBSCRIBE);

    req.set_allocated_subscribe_order_book_request(subscribeOrderBookRequest);

    const bool res = mRawGrpcClient->writeMarketDataStream(marketDataStream, req);

    if (!res)
    {
        emit authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "GrpcClient::unsubscribeOrderBook()");
    }

    return res;
}

std::shared_ptr<tinkoff::MarketDataResponse> GrpcClient::readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    std::shared_ptr<tinkoff::MarketDataResponse> resp = std::make_shared<tinkoff::MarketDataResponse>();

    if (!mRawGrpcClient->readMarketDataStream(marketDataStream, resp.get()))
    {
        // emit authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "GrpcClient::readMarketDataStream()"); // Not a problem

        return nullptr;
    }

    return resp;
}

bool GrpcClient::closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    const bool res = mRawGrpcClient->closeWriteMarketDataStream(marketDataStream);

    if (!res)
    {
        emit authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "GrpcClient::closeWriteMarketDataStream()");
    }

    return res;
}

void GrpcClient::cancelMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    marketDataStream->context.TryCancel();
}

void GrpcClient::finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    const grpc::Status status = mRawGrpcClient->finishMarketDataStream(marketDataStream);

    if (!status.ok() && status.error_code() != grpc::StatusCode::RESOURCE_EXHAUSTED &&
        status.error_code() != grpc::StatusCode::CANCELLED)
    {
        emitAuthFailed(status);
    }
}

std::shared_ptr<PortfolioStream> GrpcClient::createPortfolioStream(const QString& accountId)
{
    std::shared_ptr<PortfolioStream> res = std::make_shared<PortfolioStream>();

    tinkoff::PortfolioStreamRequest req;
    req.add_accounts(accountId.toStdString());

    res->context.set_credentials(mCreds);
    res->stream = mRawGrpcClient->createPortfolioStream(mOperationsStreamService, &res->context, req);

    return res;
}

std::shared_ptr<tinkoff::PortfolioStreamResponse>
GrpcClient::readPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream)
{
    std::shared_ptr<tinkoff::PortfolioStreamResponse> resp = std::make_shared<tinkoff::PortfolioStreamResponse>();

    if (!mRawGrpcClient->readPortfolioStream(portfolioStream, resp.get()))
    {
        // emit authFailed(grpc::StatusCode::UNKNOWN, "UNKNOWN", "", "GrpcClient::readPortfolioStream()"); // Not a problem

        return nullptr;
    }

    return resp;
}

void GrpcClient::cancelPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream)
{
    portfolioStream->context.TryCancel();
}

void GrpcClient::finishPortfolioStream(std::shared_ptr<PortfolioStream>& portfolioStream)
{
    const grpc::Status status = mRawGrpcClient->finishPortfolioStream(portfolioStream);

    if (!status.ok() && status.error_code() != grpc::StatusCode::RESOURCE_EXHAUSTED &&
        status.error_code() != grpc::StatusCode::CANCELLED)
    {
        emitAuthFailed(status);
    }
}

void GrpcClient::emitAuthFailed(const grpc::Status& status)
{
    emit authFailed(
        status.error_code(), GRPC_STATUS_CODE_TO_STRING[status.error_code()], status.error_message(), status.error_details()
    );
}
