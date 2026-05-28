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
constexpr int    OPERATIONS_LIMIT             = 1000;
constexpr qint64 MS_IN_SECOND                 = 1000LL;



// clang-format off
const QMap<grpc::StatusCode, QString> GRPC_STATUS_CODE_TO_STRING{ // clazy:exclude=non-pod-global-static
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

// clang-format off
const qint64 GRPC_PRIOIRITY_TIMEOUTS[GRPC_PRIOIRITY_COUNT] = {
    10 * MS_IN_SECOND, // GRPC_PRIOIRITY_NORMAL   - 10 seconds
    5 * MS_IN_SECOND,  // GRPC_PRIOIRITY_MINOR    - 5 seconds
    3 * MS_IN_SECOND,  // GRPC_PRIOIRITY_MAJOR    - 3 seconds
    1 * MS_IN_SECOND   // GRPC_PRIOIRITY_CRITICAL - 1 second
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
    mOrdersService           = tinkoff::OrdersService::NewStub(channel);
}

GrpcClient::~GrpcClient()
{
    qDebug() << "Destroy GrpcClient";
}

static grpc::Status getUserInfoAction(
    IRawGrpcClient*                                     rawGrpcClient,
    const std::unique_ptr<tinkoff::UsersService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&       creds,
    const tinkoff::GetInfoRequest&                      req,
    const std::shared_ptr<tinkoff::GetInfoResponse>&    resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->getUserInfo(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::GetInfoResponse> GrpcClient::getUserInfo(QThread* parentThread)
{
    const tinkoff::GetInfoRequest                   req;
    const std::shared_ptr<tinkoff::GetInfoResponse> resp = std::make_shared<tinkoff::GetInfoResponse>();

    return repeatRequest(parentThread, getUserInfoAction, mUsersService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status getAccountsAction(
    IRawGrpcClient*                                      rawGrpcClient,
    const std::unique_ptr<tinkoff::UsersService::Stub>&  service,
    const std::shared_ptr<grpc::CallCredentials>&        creds,
    const tinkoff::GetAccountsRequest&                   req,
    const std::shared_ptr<tinkoff::GetAccountsResponse>& resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->getAccounts(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::GetAccountsResponse> GrpcClient::getAccounts(QThread* parentThread)
{
    tinkoff::GetAccountsRequest                         req;
    const std::shared_ptr<tinkoff::GetAccountsResponse> resp = std::make_shared<tinkoff::GetAccountsResponse>();

    req.set_status(tinkoff::ACCOUNT_STATUS_OPEN);

    return repeatRequest(parentThread, getAccountsAction, mUsersService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status findStocksAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&             creds,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::SharesResponse>&           resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->findStocks(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::SharesResponse> GrpcClient::findStocks(QThread* parentThread, tinkoff::InstrumentStatus instrumentStatus)
{
    tinkoff::InstrumentsRequest                    req;
    const std::shared_ptr<tinkoff::SharesResponse> resp = std::make_shared<tinkoff::SharesResponse>();

    req.set_instrument_status(instrumentStatus);

    return repeatRequest(parentThread, findStocksAction, mInstrumentsService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status findBondsAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&             creds,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::BondsResponse>&            resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->findBonds(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::BondsResponse> GrpcClient::findBonds(QThread* parentThread)
{
    tinkoff::InstrumentsRequest                   req;
    const std::shared_ptr<tinkoff::BondsResponse> resp = std::make_shared<tinkoff::BondsResponse>();

    req.set_instrument_status(tinkoff::INSTRUMENT_STATUS_ALL);

    return repeatRequest(parentThread, findBondsAction, mInstrumentsService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status findCurrenciesAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&             creds,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::CurrenciesResponse>&       resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->findCurrencies(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::CurrenciesResponse> GrpcClient::findCurrencies(QThread* parentThread)
{
    tinkoff::InstrumentsRequest                        req;
    const std::shared_ptr<tinkoff::CurrenciesResponse> resp = std::make_shared<tinkoff::CurrenciesResponse>();

    req.set_instrument_status(tinkoff::INSTRUMENT_STATUS_ALL);

    return repeatRequest(parentThread, findCurrenciesAction, mInstrumentsService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status findEtfsAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&             creds,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::EtfsResponse>&             resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->findEtfs(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::EtfsResponse> GrpcClient::findEtfs(QThread* parentThread)
{
    tinkoff::InstrumentsRequest                  req;
    const std::shared_ptr<tinkoff::EtfsResponse> resp = std::make_shared<tinkoff::EtfsResponse>();

    req.set_instrument_status(tinkoff::INSTRUMENT_STATUS_ALL);

    return repeatRequest(parentThread, findEtfsAction, mInstrumentsService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status findFuturesAction(
    IRawGrpcClient*                                           rawGrpcClient,
    const std::unique_ptr<tinkoff::InstrumentsService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&             creds,
    const tinkoff::InstrumentsRequest&                        req,
    const std::shared_ptr<tinkoff::FuturesResponse>&          resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->findFutures(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::FuturesResponse> GrpcClient::findFutures(QThread* parentThread)
{
    tinkoff::InstrumentsRequest                     req;
    const std::shared_ptr<tinkoff::FuturesResponse> resp = std::make_shared<tinkoff::FuturesResponse>();

    req.set_instrument_status(tinkoff::INSTRUMENT_STATUS_ALL);

    return repeatRequest(parentThread, findFuturesAction, mInstrumentsService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status getCandlesAction(
    IRawGrpcClient*                                          rawGrpcClient,
    const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&            creds,
    const tinkoff::GetCandlesRequest&                        req,
    const std::shared_ptr<tinkoff::GetCandlesResponse>&      resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->getCandles(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::GetCandlesResponse>
GrpcClient::getCandles(QThread* parentThread, const QString& instrumentId, qint64 from, qint64 to)
{
    tinkoff::GetCandlesRequest                         req;
    const std::shared_ptr<tinkoff::GetCandlesResponse> resp = std::make_shared<tinkoff::GetCandlesResponse>();

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

    return repeatRequest(parentThread, getCandlesAction, mMarketDataService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status getOrderBookAction(
    IRawGrpcClient*                                          rawGrpcClient,
    const std::unique_ptr<tinkoff::MarketDataService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&            creds,
    const tinkoff::GetOrderBookRequest&                      req,
    const std::shared_ptr<tinkoff::GetOrderBookResponse>&    resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->getOrderBook(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::GetOrderBookResponse>
GrpcClient::getOrderBook(QThread* parentThread, const QString& instrumentId, int depth)
{
    tinkoff::GetOrderBookRequest                         req;
    const std::shared_ptr<tinkoff::GetOrderBookResponse> resp = std::make_shared<tinkoff::GetOrderBookResponse>();

    req.set_instrument_id(instrumentId.toStdString());
    req.set_depth(depth);

    return repeatRequest(parentThread, getOrderBookAction, mMarketDataService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status getPortfolioAction(
    IRawGrpcClient*                                          rawGrpcClient,
    const std::unique_ptr<tinkoff::OperationsService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&            creds,
    const tinkoff::PortfolioRequest&                         req,
    const std::shared_ptr<tinkoff::PortfolioResponse>&       resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->getPortfolio(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::PortfolioResponse> GrpcClient::getPortfolio(QThread* parentThread, const QString& accountId)
{
    tinkoff::PortfolioRequest                         req;
    const std::shared_ptr<tinkoff::PortfolioResponse> resp = std::make_shared<tinkoff::PortfolioResponse>();

    req.set_account_id(accountId.toStdString());

    return repeatRequest(parentThread, getPortfolioAction, mOperationsService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status getOperationsAction(
    IRawGrpcClient*                                                rawGrpcClient,
    const std::unique_ptr<tinkoff::OperationsService::Stub>&       service,
    const std::shared_ptr<grpc::CallCredentials>&                  creds,
    const tinkoff::GetOperationsByCursorRequest&                   req,
    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse>& resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->getOperations(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::GetOperationsByCursorResponse>
GrpcClient::getOperations(QThread* parentThread, const QString& accountId, qint64 from, qint64 to, const QString& cursor)
{
    tinkoff::GetOperationsByCursorRequest                         req;
    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> resp =
        std::make_shared<tinkoff::GetOperationsByCursorResponse>();

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

    return repeatRequest(parentThread, getOperationsAction, mOperationsService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status getMaxLotsAction(
    IRawGrpcClient*                                      rawGrpcClient,
    const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&        creds,
    const tinkoff::GetMaxLotsRequest&                    req,
    const std::shared_ptr<tinkoff::GetMaxLotsResponse>&  resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->getMaxLots(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::GetMaxLotsResponse>
GrpcClient::getMaxLots(QThread* parentThread, const QString& accountId, const QString& instrumentId, const Quotation& price)
{
    tinkoff::GetMaxLotsRequest                         req;
    const std::shared_ptr<tinkoff::GetMaxLotsResponse> resp = std::make_shared<tinkoff::GetMaxLotsResponse>();

    tinkoff::Quotation* tinkoffPrice = new tinkoff::Quotation(); // req will take ownership

    tinkoffPrice->set_units(price.units);
    tinkoffPrice->set_nano(price.nano);

    req.set_account_id(accountId.toStdString());
    req.set_instrument_id(instrumentId.toStdString());
    req.set_allocated_price(tinkoffPrice);

    return repeatRequest(parentThread, getMaxLotsAction, mOrdersService, req, resp, false, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status postOrderAction(
    IRawGrpcClient*                                      rawGrpcClient,
    const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&        creds,
    const tinkoff::PostOrderRequest&                     req,
    const std::shared_ptr<tinkoff::PostOrderResponse>&   resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->postOrder(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::PostOrderResponse> GrpcClient::postOrder(
    QThread*                parentThread,
    const QString&          accountId,
    const QString&          instrumentId,
    tinkoff::OrderDirection direction,
    qint64                  quantity,
    const Quotation&        price,
    bool                    confirmMarginTrade,
    GrpcPriority            priority
)
{
    tinkoff::PostOrderRequest                         req;
    const std::shared_ptr<tinkoff::PostOrderResponse> resp = std::make_shared<tinkoff::PostOrderResponse>();

    tinkoff::Quotation* tinkoffPrice = new tinkoff::Quotation(); // req will take ownership

    tinkoffPrice->set_units(price.units);
    tinkoffPrice->set_nano(price.nano);

    req.set_account_id(accountId.toStdString());
    req.set_instrument_id(instrumentId.toStdString());
    req.set_direction(direction);
    req.set_quantity(quantity);
    req.set_allocated_price(tinkoffPrice);
    req.set_order_type(tinkoff::ORDER_TYPE_LIMIT);
    req.set_time_in_force(tinkoff::TIME_IN_FORCE_DAY);
    req.set_price_type(tinkoff::PRICE_TYPE_CURRENCY);
    req.set_confirm_margin_trade(confirmMarginTrade);

    return repeatRequest(parentThread, postOrderAction, mOrdersService, req, resp, true, priority);
}

static grpc::Status getOrderStateAction(
    IRawGrpcClient*                                      rawGrpcClient,
    const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&        creds,
    const tinkoff::GetOrderStateRequest&                 req,
    const std::shared_ptr<tinkoff::OrderState>&          resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->getOrderState(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::OrderState>
GrpcClient::getOrderState(QThread* parentThread, const QString& accountId, const QString& orderId)
{
    tinkoff::GetOrderStateRequest              req;
    const std::shared_ptr<tinkoff::OrderState> resp = std::make_shared<tinkoff::OrderState>();

    req.set_account_id(accountId.toStdString());
    req.set_order_id(orderId.toStdString());
    req.set_price_type(tinkoff::PRICE_TYPE_CURRENCY);
    req.set_order_id_type(tinkoff::ORDER_ID_TYPE_EXCHANGE);

    return repeatRequest(parentThread, getOrderStateAction, mOrdersService, req, resp, true, GRPC_PRIOIRITY_NORMAL);
}

static grpc::Status cancelOrderAction(
    IRawGrpcClient*                                      rawGrpcClient,
    const std::unique_ptr<tinkoff::OrdersService::Stub>& service,
    const std::shared_ptr<grpc::CallCredentials>&        creds,
    const tinkoff::CancelOrderRequest&                   req,
    const std::shared_ptr<tinkoff::CancelOrderResponse>& resp
)
{
    grpc::ClientContext context;
    context.set_credentials(creds);

    return rawGrpcClient->cancelOrder(service, &context, req, resp.get());
}

std::shared_ptr<tinkoff::CancelOrderResponse>
GrpcClient::cancelOrder(QThread* parentThread, const QString& accountId, const QString& orderId)
{
    tinkoff::CancelOrderRequest                         req;
    const std::shared_ptr<tinkoff::CancelOrderResponse> resp = std::make_shared<tinkoff::CancelOrderResponse>();

    req.set_account_id(accountId.toStdString());
    req.set_order_id(orderId.toStdString());
    req.set_order_id_type(tinkoff::ORDER_ID_TYPE_EXCHANGE);

    return repeatRequest(parentThread, cancelOrderAction, mOrdersService, req, resp, true, GRPC_PRIOIRITY_CRITICAL);
}

std::shared_ptr<MarketDataStream> GrpcClient::createMarketDataStreamForLastPrice(const QStringList& instrumentIds)
{
    std::shared_ptr<MarketDataStream> res = std::make_shared<MarketDataStream>();

    tinkoff::MarketDataServerSideStreamRequest req;
    tinkoff::SubscribeLastPriceRequest*        subscribeLastPriceRequest =
        new tinkoff::SubscribeLastPriceRequest(); // req will take ownership

    subscribeLastPriceRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_SUBSCRIBE);

    for (const QString& instrumentId : instrumentIds)
    {
        subscribeLastPriceRequest->add_instruments()->set_instrument_id(instrumentId.toStdString());
    }

    req.set_allocated_subscribe_last_price_request(subscribeLastPriceRequest);

    res->context.set_credentials(mCreds);
    res->stream = mRawGrpcClient->createMarketDataStream(mMarketDataStreamService, &res->context, req);

    return res;
}

std::shared_ptr<MarketDataStream> GrpcClient::createMarketDataStreamForOrderBook(const QString& instrumentId, int depth)
{
    std::shared_ptr<MarketDataStream> res = std::make_shared<MarketDataStream>();

    tinkoff::MarketDataServerSideStreamRequest req;
    tinkoff::SubscribeOrderBookRequest*        subscribeOrderBookRequest =
        new tinkoff::SubscribeOrderBookRequest(); // req will take ownership

    subscribeOrderBookRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_SUBSCRIBE);
    tinkoff::OrderBookInstrument* orderBook = subscribeOrderBookRequest->add_instruments();

    orderBook->set_instrument_id(instrumentId.toStdString());
    orderBook->set_depth(depth);
    orderBook->set_order_book_type(tinkoff::ORDERBOOK_TYPE_ALL);

    req.set_allocated_subscribe_order_book_request(subscribeOrderBookRequest);

    res->context.set_credentials(mCreds);
    res->stream = mRawGrpcClient->createMarketDataStream(mMarketDataStreamService, &res->context, req);

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

void GrpcClient::cancelMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    marketDataStream->context.TryCancel();
}

void GrpcClient::finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    const grpc::Status     status    = mRawGrpcClient->finishMarketDataStream(marketDataStream);
    const grpc::StatusCode errorCode = status.error_code();

    if (!status.ok() && errorCode != grpc::StatusCode::RESOURCE_EXHAUSTED && errorCode != grpc::StatusCode::UNKNOWN &&
        errorCode != grpc::StatusCode::CANCELLED)
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

std::shared_ptr<PortfolioStream> GrpcClient::createPortfolioStream(const QString& accountId, const QString& anotherAccountId)
{
    std::shared_ptr<PortfolioStream> res = std::make_shared<PortfolioStream>();

    tinkoff::PortfolioStreamRequest req;
    req.add_accounts(accountId.toStdString());
    req.add_accounts(anotherAccountId.toStdString());

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
    const grpc::Status     status    = mRawGrpcClient->finishPortfolioStream(portfolioStream);
    const grpc::StatusCode errorCode = status.error_code();

    if (!status.ok() && errorCode != grpc::StatusCode::RESOURCE_EXHAUSTED && errorCode != grpc::StatusCode::UNKNOWN &&
        errorCode != grpc::StatusCode::CANCELLED)
    {
        emitAuthFailed(status);
    }
}

void GrpcClient::emitAuthFailed(const grpc::Status& status)
{
    emit authFailed(GRPC_STATUS_CODE_TO_STRING[status.error_code()]);
}
