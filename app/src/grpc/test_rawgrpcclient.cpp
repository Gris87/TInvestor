#include "src/grpc/rawgrpcclient.h"

#include <grpcpp/grpcpp.h>
#include <gtest/gtest.h>

#include "src/domain/user/user.h"
#include "src/grpc/investapiuthenticator.h"
#include "src/storage/user/iuserstorage_mock.h"



#define HHRU_UID "cf9ed7ef-784d-4c2c-aabe-362891fcd80c"
#define SPBE_UID "15dc2120-29d2-48b8-87c0-da1d95255f68"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_RawGrpcClient : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        creds = grpc::MetadataCredentialsFromPlugin(
            std::unique_ptr<grpc::MetadataCredentialsPlugin>(new InvestApiAuthenticator(userStorageMock))
        );

        std::shared_ptr<grpc::Channel> channel =
            grpc::CreateChannel(ADDRESS, grpc::SslCredentials(grpc::SslCredentialsOptions()));

        usersService            = tinkoff::UsersService::NewStub(channel);
        instrumentsService      = tinkoff::InstrumentsService::NewStub(channel);
        marketDataService       = tinkoff::MarketDataService::NewStub(channel);
        marketDataStreamService = tinkoff::MarketDataStreamService::NewStub(channel);

        client = new RawGrpcClient();
    }

    void TearDown() override
    {
        delete client;
        delete userStorageMock;
    }

    RawGrpcClient*                                          client;
    StrictMock<UserStorageMock>*                            userStorageMock;
    std::shared_ptr<grpc::CallCredentials>                  creds;
    std::unique_ptr<tinkoff::UsersService::Stub>            usersService;
    std::unique_ptr<tinkoff::InstrumentsService::Stub>      instrumentsService;
    std::unique_ptr<tinkoff::MarketDataService::Stub>       marketDataService;
    std::unique_ptr<tinkoff::MarketDataStreamService::Stub> marketDataStreamService;
};



TEST_F(Test_RawGrpcClient, Test_constructor_and_destructor)
{
}

TEST_F(Test_RawGrpcClient, Test_getUserInfo)
{
    const InSequence seq;

    grpc::ClientContext                       context;
    tinkoff::GetInfoRequest                   req;
    std::shared_ptr<tinkoff::GetInfoResponse> resp = std::shared_ptr<tinkoff::GetInfoResponse>(new tinkoff::GetInfoResponse());

    context.set_credentials(creds);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    grpc::Status status = client->getUserInfo(usersService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),                          true);
    ASSERT_EQ(resp->prem_status(),                  false);
    ASSERT_EQ(resp->qual_status(),                  false);
    ASSERT_EQ(resp->qualified_for_work_with_size(), 11);
    ASSERT_EQ(resp->qualified_for_work_with(0),     "derivative");
    ASSERT_EQ(resp->qualified_for_work_with(1),     "structured_bonds");
    ASSERT_EQ(resp->qualified_for_work_with(2),     "closed_fund");
    ASSERT_EQ(resp->qualified_for_work_with(3),     "bond");
    ASSERT_EQ(resp->qualified_for_work_with(4),     "structured_income_bonds");
    ASSERT_EQ(resp->qualified_for_work_with(5),     "russian_shares");
    ASSERT_EQ(resp->qualified_for_work_with(6),     "leverage");
    ASSERT_EQ(resp->qualified_for_work_with(7),     "foreign_shares");
    ASSERT_EQ(resp->qualified_for_work_with(8),     "foreign_etf");
    ASSERT_EQ(resp->qualified_for_work_with(9),     "foreign_bond");
    ASSERT_EQ(resp->qualified_for_work_with(10),    "option");
    ASSERT_EQ(resp->tariff(),                       "sandbox");
    ASSERT_EQ(resp->user_id(),                      "");
    ASSERT_EQ(resp->risk_level_code(),              "");
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_getAccounts)
{
    const InSequence seq;

    grpc::ClientContext                           context;
    tinkoff::GetAccountsRequest                   req;
    std::shared_ptr<tinkoff::GetAccountsResponse> resp =
        std::shared_ptr<tinkoff::GetAccountsResponse>(new tinkoff::GetAccountsResponse());

    context.set_credentials(creds);

    req.set_status(tinkoff::ACCOUNT_STATUS_OPEN);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    grpc::Status status = client->getAccounts(usersService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),                               true);
    ASSERT_EQ(resp->accounts_size(),                     2);
    ASSERT_EQ(resp->accounts(0).id(),                    "1c3c3978-dc5b-4010-af6e-37d320122e58");
    ASSERT_EQ(resp->accounts(0).type(),                  tinkoff::ACCOUNT_TYPE_TINKOFF);
    ASSERT_EQ(resp->accounts(0).name(),                  "Sandboxer");
    ASSERT_EQ(resp->accounts(0).status(),                tinkoff::ACCOUNT_STATUS_OPEN);
    ASSERT_EQ(resp->accounts(0).opened_date().seconds(), 1740053879);
    ASSERT_EQ(resp->accounts(0).closed_date().seconds(), 0);
    ASSERT_EQ(resp->accounts(0).access_level(),          tinkoff::ACCOUNT_ACCESS_LEVEL_FULL_ACCESS);
    ASSERT_EQ(resp->accounts(1).id(),                    "1928101b-6bf3-4174-a03c-b44c8b7c046d");
    ASSERT_EQ(resp->accounts(1).type(),                  tinkoff::ACCOUNT_TYPE_TINKOFF);
    ASSERT_EQ(resp->accounts(1).name(),                  "My dear cash");
    ASSERT_EQ(resp->accounts(1).status(),                tinkoff::ACCOUNT_STATUS_OPEN);
    ASSERT_EQ(resp->accounts(1).opened_date().seconds(), 1740053921);
    ASSERT_EQ(resp->accounts(1).closed_date().seconds(), 0);
    ASSERT_EQ(resp->accounts(1).access_level(),          tinkoff::ACCOUNT_ACCESS_LEVEL_FULL_ACCESS);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_findStocks)
{
    const InSequence seq;

    grpc::ClientContext                      context;
    tinkoff::InstrumentsRequest              req;
    std::shared_ptr<tinkoff::SharesResponse> resp = std::shared_ptr<tinkoff::SharesResponse>(new tinkoff::SharesResponse());

    context.set_credentials(creds);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    grpc::Status status = client->findStocks(instrumentsService, &context, req, resp.get());

    int index = -1;

    for (int i = 0; i < resp->instruments_size(); ++i)
    {
        if (resp->instruments(i).uid() == HHRU_UID)
        {
            index = i;

            break;
        }
    }

    // clang-format off
    ASSERT_EQ(status.ok(),                                                 true);
    ASSERT_GE(resp->instruments_size(),                                    166);
    ASSERT_GE(index,                                                       0);
    ASSERT_EQ(resp->instruments(index).figi(),                             "TCS2207L1061");
    ASSERT_EQ(resp->instruments(index).ticker(),                           "HHRU");
    ASSERT_EQ(resp->instruments(index).class_code(),                       "TQBR");
    ASSERT_EQ(resp->instruments(index).isin(),                             "US42207L1061");
    ASSERT_EQ(resp->instruments(index).lot(),                              1);
    ASSERT_EQ(resp->instruments(index).currency(),                         "rub");
    ASSERT_EQ(resp->instruments(index).dlong().units(),                    0);
    ASSERT_EQ(resp->instruments(index).dlong().nano(),                     0);
    ASSERT_EQ(resp->instruments(index).dshort().units(),                   0);
    ASSERT_EQ(resp->instruments(index).dshort().nano(),                    0);
    ASSERT_EQ(resp->instruments(index).dlong_min().units(),                0);
    ASSERT_EQ(resp->instruments(index).dlong_min().nano(),                 0);
    ASSERT_EQ(resp->instruments(index).dshort_min().units(),               0);
    ASSERT_EQ(resp->instruments(index).dshort_min().nano(),                0);
    ASSERT_EQ(resp->instruments(index).short_enabled_flag(),               false);
    ASSERT_EQ(resp->instruments(index).name(),                             "HeadHunter Group PLC");
    ASSERT_EQ(resp->instruments(index).exchange(),                         "unknown");
    ASSERT_EQ(resp->instruments(index).ipo_date().seconds(),               0);
    ASSERT_EQ(resp->instruments(index).issue_size(),                       50635720);
    ASSERT_EQ(resp->instruments(index).country_of_risk(),                  "RU");
    ASSERT_EQ(resp->instruments(index).country_of_risk_name(),             "\u0420\u043e\u0441\u0441\u0438\u0439\u0441\u043a\u0430\u044f\u0020\u0424\u0435\u0434\u0435\u0440\u0430\u0446\u0438\u044f"); // Russian Federation
    ASSERT_EQ(resp->instruments(index).sector(),                           "it");
    ASSERT_EQ(resp->instruments(index).issue_size_plan(),                  0);
    ASSERT_EQ(resp->instruments(index).nominal().units(),                  1);
    ASSERT_EQ(resp->instruments(index).nominal().nano(),                   0);
    ASSERT_EQ(resp->instruments(index).trading_status(),                   tinkoff::SECURITY_TRADING_STATUS_NOT_AVAILABLE_FOR_TRADING);
    ASSERT_EQ(resp->instruments(index).otc_flag(),                         false);
    ASSERT_EQ(resp->instruments(index).buy_available_flag(),               false);
    ASSERT_EQ(resp->instruments(index).sell_available_flag(),              false);
    ASSERT_EQ(resp->instruments(index).div_yield_flag(),                   true);
    ASSERT_EQ(resp->instruments(index).share_type(),                       tinkoff::SHARE_TYPE_ADR);
    ASSERT_EQ(resp->instruments(index).min_price_increment().units(),      1);
    ASSERT_EQ(resp->instruments(index).min_price_increment().nano(),       0);
    ASSERT_EQ(resp->instruments(index).api_trade_available_flag(),         true);
    ASSERT_EQ(resp->instruments(index).uid(),                              HHRU_UID);
    ASSERT_EQ(resp->instruments(index).real_exchange(),                    tinkoff::REAL_EXCHANGE_MOEX);
    ASSERT_EQ(resp->instruments(index).position_uid(),                     "d2778f75-bf3e-4731-99ea-45f9b2947fdb");
    ASSERT_EQ(resp->instruments(index).asset_uid(),                        "3245edc9-0713-4b1e-8e9f-7bdd10a8d605");
    ASSERT_EQ(resp->instruments(index).instrument_exchange(),              0);
    ASSERT_EQ(resp->instruments(index).for_iis_flag(),                     false);
    ASSERT_EQ(resp->instruments(index).for_qual_investor_flag(),           false);
    ASSERT_EQ(resp->instruments(index).weekend_flag(),                     false);
    ASSERT_EQ(resp->instruments(index).blocked_tca_flag(),                 false);
    ASSERT_EQ(resp->instruments(index).liquidity_flag(),                   true);
    ASSERT_EQ(resp->instruments(index).first_1min_candle_date().seconds(), 1601017140);
    ASSERT_EQ(resp->instruments(index).first_1day_candle_date().seconds(), 1601017200);
    ASSERT_EQ(resp->instruments(index).brand().logo_name(),                "US42207L1061.png");
    ASSERT_EQ(resp->instruments(index).brand().logo_base_color(),          "#d1151a");
    ASSERT_EQ(resp->instruments(index).brand().text_color(),               "#ffffff");
    ASSERT_EQ(resp->instruments(index).dlong_client().units(),             0);
    ASSERT_EQ(resp->instruments(index).dshort_client().nano(),             0);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_getCandles)
{
    const InSequence seq;

    grpc::ClientContext                          context;
    tinkoff::GetCandlesRequest                   req;
    std::shared_ptr<tinkoff::GetCandlesResponse> resp =
        std::shared_ptr<tinkoff::GetCandlesResponse>(new tinkoff::GetCandlesResponse());

    context.set_credentials(creds);

    ::google::protobuf::Timestamp* fromTimestamp = new ::google::protobuf::Timestamp(); // req will take ownership
    ::google::protobuf::Timestamp* toTimestamp   = new ::google::protobuf::Timestamp(); // req will take ownership

    fromTimestamp->set_seconds(1741935600);
    fromTimestamp->set_nanos(0);
    toTimestamp->set_seconds(1741935660);
    toTimestamp->set_nanos(0);

    req.set_instrument_id(SPBE_UID);
    req.set_allocated_from(fromTimestamp);
    req.set_allocated_to(toTimestamp);
    req.set_interval(tinkoff::CANDLE_INTERVAL_1_MIN);
    req.set_limit(MAX_LIMIT_FOR_INTERVAL_1_MIN);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    grpc::Status status = client->getCandles(marketDataService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),                       true);
    ASSERT_GE(resp->candles_size(),              1);
    ASSERT_EQ(resp->candles(0).open().units(),   292);
    ASSERT_EQ(resp->candles(0).open().nano(),    800000000);
    ASSERT_EQ(resp->candles(0).high().units(),   293);
    ASSERT_EQ(resp->candles(0).high().nano(),    500000000);
    ASSERT_EQ(resp->candles(0).low().units(),    292);
    ASSERT_EQ(resp->candles(0).low().nano(),     800000000);
    ASSERT_EQ(resp->candles(0).close().units(),  293);
    ASSERT_EQ(resp->candles(0).close().nano(),   500000000);
    ASSERT_EQ(resp->candles(0).volume(),         3173);
    ASSERT_EQ(resp->candles(0).time().seconds(), 1741935600);
    ASSERT_EQ(resp->candles(0).is_complete(),    true);
    ASSERT_EQ(resp->candles(0).candle_source(),  tinkoff::CANDLE_SOURCE_EXCHANGE);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_getOrderBook)
{
    const InSequence seq;

    grpc::ClientContext                            context;
    tinkoff::GetOrderBookRequest                   req;
    std::shared_ptr<tinkoff::GetOrderBookResponse> resp =
        std::shared_ptr<tinkoff::GetOrderBookResponse>(new tinkoff::GetOrderBookResponse());

    context.set_credentials(creds);

    req.set_instrument_id(SPBE_UID);
    req.set_depth(50);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    grpc::Status status = client->getOrderBook(marketDataService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),            true);
    ASSERT_EQ(resp->depth(),          50);
    ASSERT_EQ(resp->instrument_uid(), SPBE_UID);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_MarketDataStream)
{
    const InSequence seq;

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    std::shared_ptr<MarketDataStream> marketDataStream(new MarketDataStream());

    marketDataStream->context.set_credentials(creds);
    marketDataStream->stream = client->createMarketDataStream(marketDataStreamService, &marketDataStream->context);

    ASSERT_NE(marketDataStream->stream, nullptr);

    tinkoff::MarketDataRequest          subscribeRequest;
    tinkoff::SubscribeLastPriceRequest* subscribeLastPriceRequest =
        new tinkoff::SubscribeLastPriceRequest(); // subscribeRequest will take ownership

    subscribeLastPriceRequest->set_subscription_action(tinkoff::SUBSCRIPTION_ACTION_SUBSCRIBE);
    subscribeLastPriceRequest->add_instruments()->set_instrument_id(SPBE_UID);

    subscribeRequest.set_allocated_subscribe_last_price_request(subscribeLastPriceRequest);

    ASSERT_EQ(client->writeMarketDataStream(marketDataStream, subscribeRequest), true);

    tinkoff::MarketDataResponse subscribeResponse;

    ASSERT_EQ(client->readMarketDataStream(marketDataStream, &subscribeResponse), true);

    // clang-format off
    ASSERT_EQ(subscribeResponse.has_subscribe_last_price_response(),                                               true);
    ASSERT_NE(subscribeResponse.subscribe_last_price_response().tracking_id(),                                     "");
    ASSERT_EQ(subscribeResponse.subscribe_last_price_response().last_price_subscriptions_size(),                   1);
    ASSERT_EQ(subscribeResponse.subscribe_last_price_response().last_price_subscriptions(0).figi(),                "TCS60A0JQ9P9");
    ASSERT_EQ(subscribeResponse.subscribe_last_price_response().last_price_subscriptions(0).subscription_status(), tinkoff::SUBSCRIPTION_STATUS_SUCCESS);
    ASSERT_EQ(subscribeResponse.subscribe_last_price_response().last_price_subscriptions(0).instrument_uid(),      SPBE_UID);
    ASSERT_NE(subscribeResponse.subscribe_last_price_response().last_price_subscriptions(0).stream_id(),           "");
    ASSERT_NE(subscribeResponse.subscribe_last_price_response().last_price_subscriptions(0).subscription_id(),     "");
    // clang-format on

    // clang-format off
    ASSERT_EQ(client->closeWriteMarketDataStream(marketDataStream),          true);
    ASSERT_EQ(client->finishMarketDataStream(marketDataStream).error_code(), grpc::StatusCode::OK);
    // clang-format on
}
