#include "src/grpc/grpcclient.h"

#include <QDebug>
#include <grpcpp/grpcpp.h>

#include "src/grpc/investapiuthenticator.h"



#ifndef USE_SANDBOX
const char* GRPC_ADDRESS = "invest-public-api.tinkoff.ru:443";
#else
const char* GRPC_ADDRESS = "sandbox-invest-public-api.tinkoff.ru:443";
#endif

constexpr int MAX_LIMIT_FOR_INTERVAL_1_MIN = 2400;
constexpr int MS_IN_SECOND                 = 1000;



#define REPEAT_REQUEST(parentThread, timeUtils, rawGrpcClient, action, service, context, req, resp)   \
    while (true)                                                                                      \
    {                                                                                                 \
        grpc::Status status = rawGrpcClient->action(service, &context, req, resp.get());              \
                                                                                                      \
        if (!parentThread->isInterruptionRequested() && !status.ok())                                 \
        {                                                                                             \
            if (status.error_code() == grpc::StatusCode::RESOURCE_EXHAUSTED)                          \
            {                                                                                         \
                if (timeUtils->interruptibleSleep(5000, parentThread))                                \
                {                                                                                     \
                    return nullptr;                                                                   \
                }                                                                                     \
                                                                                                      \
                continue;                                                                             \
            }                                                                                         \
                                                                                                      \
            if (status.error_code() != grpc::StatusCode::CANCELLED)                                   \
            {                                                                                         \
                emit authFailed(status.error_code(), status.error_message(), status.error_details()); \
            }                                                                                         \
                                                                                                      \
            return nullptr;                                                                           \
        }                                                                                             \
                                                                                                      \
        break;                                                                                        \
    }



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
}

GrpcClient::~GrpcClient()
{
    qDebug() << "Destroy GrpcClient";
}

std::shared_ptr<tinkoff::GetInfoResponse> GrpcClient::getUserInfo(QThread* parentThread)
{
    grpc::ClientContext                       context;
    const tinkoff::GetInfoRequest             req;
    std::shared_ptr<tinkoff::GetInfoResponse> resp = std::shared_ptr<tinkoff::GetInfoResponse>(new tinkoff::GetInfoResponse());

    context.set_credentials(mCreds);

    REPEAT_REQUEST(parentThread, mTimeUtils, mRawGrpcClient, getUserInfo, mUsersService, context, req, resp);

    return resp;
}

std::shared_ptr<tinkoff::GetAccountsResponse> GrpcClient::getAccounts(QThread* parentThread)
{
    grpc::ClientContext                           context;
    tinkoff::GetAccountsRequest                   req;
    std::shared_ptr<tinkoff::GetAccountsResponse> resp =
        std::shared_ptr<tinkoff::GetAccountsResponse>(new tinkoff::GetAccountsResponse());

    context.set_credentials(mCreds);

    req.set_status(tinkoff::ACCOUNT_STATUS_OPEN);

    REPEAT_REQUEST(parentThread, mTimeUtils, mRawGrpcClient, getAccounts, mUsersService, context, req, resp);

    return resp;
}

std::shared_ptr<tinkoff::SharesResponse> GrpcClient::findStocks(QThread* parentThread)
{
    grpc::ClientContext                      context;
    tinkoff::InstrumentsRequest              req;
    std::shared_ptr<tinkoff::SharesResponse> resp = std::shared_ptr<tinkoff::SharesResponse>(new tinkoff::SharesResponse());

    context.set_credentials(mCreds);

    REPEAT_REQUEST(parentThread, mTimeUtils, mRawGrpcClient, findStocks, mInstrumentsService, context, req, resp);

    return resp;
}

std::shared_ptr<tinkoff::GetCandlesResponse>
GrpcClient::getCandles(QThread* parentThread, const QString& uid, qint64 from, qint64 to)
{
    grpc::ClientContext                          context;
    tinkoff::GetCandlesRequest                   req;
    std::shared_ptr<tinkoff::GetCandlesResponse> resp =
        std::shared_ptr<tinkoff::GetCandlesResponse>(new tinkoff::GetCandlesResponse());

    context.set_credentials(mCreds);

    ::google::protobuf::Timestamp* fromTimestamp = new ::google::protobuf::Timestamp(); // req will take ownership
    ::google::protobuf::Timestamp* toTimestamp   = new ::google::protobuf::Timestamp(); // req will take ownership

    fromTimestamp->set_seconds(from / MS_IN_SECOND);
    fromTimestamp->set_nanos(0);
    toTimestamp->set_seconds(to / MS_IN_SECOND);
    toTimestamp->set_nanos(0);

    req.set_instrument_id(uid.toStdString());
    req.set_allocated_from(fromTimestamp);
    req.set_allocated_to(toTimestamp);
    req.set_interval(tinkoff::CANDLE_INTERVAL_1_MIN);
    req.set_limit(MAX_LIMIT_FOR_INTERVAL_1_MIN);

    REPEAT_REQUEST(parentThread, mTimeUtils, mRawGrpcClient, getCandles, mMarketDataService, context, req, resp);

    return resp;
}

std::shared_ptr<tinkoff::GetOrderBookResponse> GrpcClient::getOrderBook(QThread* parentThread, const QString& uid)
{
    grpc::ClientContext                            context;
    tinkoff::GetOrderBookRequest                   req;
    std::shared_ptr<tinkoff::GetOrderBookResponse> resp =
        std::shared_ptr<tinkoff::GetOrderBookResponse>(new tinkoff::GetOrderBookResponse());

    context.set_credentials(mCreds);

    req.set_instrument_id(uid.toStdString());
    req.set_depth(50);

    REPEAT_REQUEST(parentThread, mTimeUtils, mRawGrpcClient, getOrderBook, mMarketDataService, context, req, resp);

    return resp;
}

std::shared_ptr<MarketDataStream> GrpcClient::createMarketDataStream()
{
    std::shared_ptr<MarketDataStream> res(new MarketDataStream());

    res->context.set_credentials(mCreds);
    res->stream = mRawGrpcClient->createMarketDataStream(mMarketDataStreamService, &res->context);

    return res;
}

bool GrpcClient::subscribeLastPrices(std::shared_ptr<MarketDataStream>& marketDataStream, const QStringList& uids)
{
    tinkoff::MarketDataRequest          req;
    tinkoff::SubscribeLastPriceRequest* subscribeLastPriceRequest =
        new tinkoff::SubscribeLastPriceRequest(); // req will take ownership

    subscribeLastPriceRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_SUBSCRIBE);

    for (int i = 0; i < uids.size(); ++i)
    {
        subscribeLastPriceRequest->add_instruments()->set_instrument_id(uids.at(i).toStdString());
    }

    req.set_allocated_subscribe_last_price_request(subscribeLastPriceRequest);

    const bool res = mRawGrpcClient->writeMarketDataStream(marketDataStream, req);

    if (!res)
    {
        emit authFailed(grpc::StatusCode::UNKNOWN, "", "");
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
        emit authFailed(grpc::StatusCode::UNKNOWN, "", "");
    }

    return res;
}

bool GrpcClient::subscribeOrderBook(std::shared_ptr<MarketDataStream>& marketDataStream, const QString& uid)
{
    tinkoff::MarketDataRequest          req;
    tinkoff::SubscribeOrderBookRequest* subscribeOrderBookRequest =
        new tinkoff::SubscribeOrderBookRequest(); // req will take ownership

    subscribeOrderBookRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_SUBSCRIBE);
    tinkoff::OrderBookInstrument* orderBook = subscribeOrderBookRequest->add_instruments();

    orderBook->set_instrument_id(uid.toStdString());
    orderBook->set_depth(50);
    orderBook->set_order_book_type(tinkoff::ORDERBOOK_TYPE_ALL);

    req.set_allocated_subscribe_order_book_request(subscribeOrderBookRequest);

    const bool res = mRawGrpcClient->writeMarketDataStream(marketDataStream, req);

    if (!res)
    {
        emit authFailed(grpc::StatusCode::UNKNOWN, "", "");
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
        emit authFailed(grpc::StatusCode::UNKNOWN, "", "");
    }

    return res;
}

std::shared_ptr<tinkoff::MarketDataResponse> GrpcClient::readMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    std::shared_ptr<tinkoff::MarketDataResponse> resp =
        std::shared_ptr<tinkoff::MarketDataResponse>(new tinkoff::MarketDataResponse());

    if (!mRawGrpcClient->readMarketDataStream(marketDataStream, resp.get()))
    {
        // emit authFailed(grpc::StatusCode::UNKNOWN, "", ""); // Not a problem

        return nullptr;
    }

    return resp;
}

bool GrpcClient::closeWriteMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    const bool res = mRawGrpcClient->closeWriteMarketDataStream(marketDataStream);

    if (!res)
    {
        emit authFailed(grpc::StatusCode::UNKNOWN, "", "");
    }

    return res;
}

void GrpcClient::finishMarketDataStream(std::shared_ptr<MarketDataStream>& marketDataStream)
{
    const grpc::Status status = mRawGrpcClient->finishMarketDataStream(marketDataStream);

    if (!status.ok())
    {
        emit authFailed(status.error_code(), status.error_message(), status.error_details());
    }
}
