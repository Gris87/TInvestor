#include "src/grpc/rawgrpcclient.h"

#include <QDebug>
#include <grpcpp/grpcpp.h>
#include <gtest/gtest.h>
#include <memory>

#include "src/domain/user/user.h"
#include "src/grpc/investapiuthenticator.h"
#include "src/storage/user/iuserstorage_mock.h"



const char* const GRPC_ADDRESS = "sandbox-invest-public-api.tinkoff.ru:443";
const char* const STOCK_UID    = "cf9ed7ef-784d-4c2c-aabe-362891fcd80c";
const char* const BOND_UID     = "2c7c296c-1f26-429f-ad8b-03fa1b6ba7fa";
const char* const CURRENCY_UID = "a22a1263-8e1b-4546-a1aa-416463f104d3";
const char* const ETF_UID      = "db147f62-8909-44d7-bd73-368ad48dc890";
const char* const FUTURE_UID   = "75d7d665-c378-4685-b428-eb297314795a";
const char* const SPBE_UID     = "15dc2120-29d2-48b8-87c0-da1d95255f68";

constexpr int MAX_LIMIT_FOR_INTERVAL_1_MIN = 2400;



using ::testing::InSequence;
using ::testing::ReturnRef;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
class Test_RawGrpcClient : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        creds = grpc::MetadataCredentialsFromPlugin(
            std::unique_ptr<grpc::MetadataCredentialsPlugin>(new InvestApiAuthenticator(userStorageMock))
        );

        const std::shared_ptr<grpc::Channel> channel =
            grpc::CreateChannel(GRPC_ADDRESS, grpc::SslCredentials(grpc::SslCredentialsOptions()));

        usersService            = tinkoff::UsersService::NewStub(channel);
        instrumentsService      = tinkoff::InstrumentsService::NewStub(channel);
        marketDataService       = tinkoff::MarketDataService::NewStub(channel);
        marketDataStreamService = tinkoff::MarketDataStreamService::NewStub(channel);
        operationsService       = tinkoff::OperationsService::NewStub(channel);
        operationsStreamService = tinkoff::OperationsStreamService::NewStub(channel);
        ordersService           = tinkoff::OrdersService::NewStub(channel);

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
    std::unique_ptr<tinkoff::OperationsService::Stub>       operationsService;
    std::unique_ptr<tinkoff::OperationsStreamService::Stub> operationsStreamService;
    std::unique_ptr<tinkoff::OrdersService::Stub>           ordersService;
};



TEST_F(Test_RawGrpcClient, Test_constructor_and_destructor)
{
}

TEST_F(Test_RawGrpcClient, Test_getUserInfo)
{
    const InSequence seq;

    grpc::ClientContext                             context;
    const tinkoff::GetInfoRequest                   req;
    const std::shared_ptr<tinkoff::GetInfoResponse> resp = std::make_shared<tinkoff::GetInfoResponse>();

    context.set_credentials(creds);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->getUserInfo(usersService, &context, req, resp.get());

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

    grpc::ClientContext                                 context;
    tinkoff::GetAccountsRequest                         req;
    const std::shared_ptr<tinkoff::GetAccountsResponse> resp = std::make_shared<tinkoff::GetAccountsResponse>();

    context.set_credentials(creds);

    req.set_status(tinkoff::ACCOUNT_STATUS_OPEN);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->getAccounts(usersService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),                               true);
    ASSERT_EQ(resp->accounts_size(),                     2);
    ASSERT_EQ(resp->accounts(0).id(),                    "d1843f24-0864-4666-8608-d5d16822fbae");
    ASSERT_EQ(resp->accounts(0).type(),                  tinkoff::ACCOUNT_TYPE_TINKOFF);
    ASSERT_EQ(resp->accounts(0).name(),                  "Hello darkness smile face");
    ASSERT_EQ(resp->accounts(0).status(),                tinkoff::ACCOUNT_STATUS_OPEN);
    ASSERT_EQ(resp->accounts(0).opened_date().seconds(), 1748173834);
    ASSERT_EQ(resp->accounts(0).closed_date().seconds(), 0);
    ASSERT_EQ(resp->accounts(0).access_level(),          tinkoff::ACCOUNT_ACCESS_LEVEL_FULL_ACCESS);
    ASSERT_EQ(resp->accounts(1).id(),                    "8963c33d-7ee2-4ff2-ac01-8c090ec502f5");
    ASSERT_EQ(resp->accounts(1).type(),                  tinkoff::ACCOUNT_TYPE_TINKOFF);
    ASSERT_EQ(resp->accounts(1).name(),                  "BlahBlahCar");
    ASSERT_EQ(resp->accounts(1).status(),                tinkoff::ACCOUNT_STATUS_OPEN);
    ASSERT_EQ(resp->accounts(1).opened_date().seconds(), 1748173847);
    ASSERT_EQ(resp->accounts(1).closed_date().seconds(), 0);
    ASSERT_EQ(resp->accounts(1).access_level(),          tinkoff::ACCOUNT_ACCESS_LEVEL_FULL_ACCESS);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_findStocks)
{
    const InSequence seq;

    grpc::ClientContext                            context;
    const tinkoff::InstrumentsRequest              req;
    const std::shared_ptr<tinkoff::SharesResponse> resp = std::make_shared<tinkoff::SharesResponse>();

    context.set_credentials(creds);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->findStocks(instrumentsService, &context, req, resp.get());

    int index = -1;

    for (int i = 0; i < resp->instruments_size(); ++i)
    {
        if (resp->instruments(i).uid() == STOCK_UID)
        {
            index = i;

            break;
        }
    }

    // clang-format off
    ASSERT_EQ(status.ok(),                                                 true);
    ASSERT_GE(resp->instruments_size(),                                    1955);
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
    ASSERT_EQ(resp->instruments(index).country_of_risk_name(),             "\u0420\u043E\u0441\u0441\u0438\u0439\u0441\u043A\u0430\u044F \u0424\u0435\u0434\u0435\u0440\u0430\u0446\u0438\u044F"); // Russian Federation
    ASSERT_EQ(resp->instruments(index).sector(),                           "it");
    ASSERT_EQ(resp->instruments(index).issue_size_plan(),                  0);
    ASSERT_EQ(resp->instruments(index).nominal().currency(),               "usd");
    ASSERT_EQ(resp->instruments(index).nominal().units(),                  1);
    ASSERT_EQ(resp->instruments(index).nominal().nano(),                   0);
    // ASSERT_EQ(resp->instruments(index).trading_status(),                   tinkoff::SECURITY_TRADING_STATUS_NOT_AVAILABLE_FOR_TRADING);
    ASSERT_EQ(resp->instruments(index).otc_flag(),                         false);
    ASSERT_EQ(resp->instruments(index).buy_available_flag(),               false);
    ASSERT_EQ(resp->instruments(index).sell_available_flag(),              false);
    ASSERT_EQ(resp->instruments(index).div_yield_flag(),                   true);
    ASSERT_EQ(resp->instruments(index).share_type(),                       tinkoff::SHARE_TYPE_ADR);
    ASSERT_EQ(resp->instruments(index).min_price_increment().units(),      1);
    ASSERT_EQ(resp->instruments(index).min_price_increment().nano(),       0);
    ASSERT_EQ(resp->instruments(index).api_trade_available_flag(),         true);
    ASSERT_EQ(resp->instruments(index).uid(),                              STOCK_UID);
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
    ASSERT_EQ(resp->instruments(index).dlong_client().nano(),              0);
    ASSERT_EQ(resp->instruments(index).dshort_client().units(),            0);
    ASSERT_EQ(resp->instruments(index).dshort_client().nano(),             0);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_findBonds)
{
    const InSequence seq;

    grpc::ClientContext                           context;
    const tinkoff::InstrumentsRequest             req;
    const std::shared_ptr<tinkoff::BondsResponse> resp = std::make_shared<tinkoff::BondsResponse>();

    context.set_credentials(creds);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->findBonds(instrumentsService, &context, req, resp.get());

    int index = -1;

    for (int i = 0; i < resp->instruments_size(); ++i)
    {
        if (resp->instruments(i).uid() == BOND_UID)
        {
            index = i;

            break;
        }
    }

    // clang-format off
    ASSERT_EQ(status.ok(),                                                 true);
    ASSERT_GE(resp->instruments_size(),                                    1291);
    ASSERT_GE(index,                                                       0);
    ASSERT_EQ(resp->instruments(index).figi(),                             "BBG0000JWX54");
    ASSERT_EQ(resp->instruments(index).ticker(),                           "XS0316524130");
    ASSERT_EQ(resp->instruments(index).class_code(),                       "TQOD");
    ASSERT_EQ(resp->instruments(index).isin(),                             "XS0316524130");
    ASSERT_EQ(resp->instruments(index).lot(),                              1);
    ASSERT_EQ(resp->instruments(index).currency(),                         "usd");
    ASSERT_EQ(resp->instruments(index).dlong().units(),                    0);
    ASSERT_EQ(resp->instruments(index).dlong().nano(),                     0);
    ASSERT_EQ(resp->instruments(index).dshort().units(),                   0);
    ASSERT_EQ(resp->instruments(index).dshort().nano(),                    0);
    ASSERT_EQ(resp->instruments(index).dlong_min().units(),                0);
    ASSERT_EQ(resp->instruments(index).dlong_min().nano(),                 0);
    ASSERT_EQ(resp->instruments(index).dshort_min().units(),               0);
    ASSERT_EQ(resp->instruments(index).dshort_min().nano(),                0);
    ASSERT_EQ(resp->instruments(index).short_enabled_flag(),               false);
    ASSERT_EQ(resp->instruments(index).name(),                             "Gazprom");
    ASSERT_EQ(resp->instruments(index).exchange(),                         "unknown");
    ASSERT_EQ(resp->instruments(index).coupon_quantity_per_year(),         2);
    ASSERT_EQ(resp->instruments(index).maturity_date().seconds(),          2133993600);
    ASSERT_EQ(resp->instruments(index).nominal().currency(),               "usd");
    ASSERT_EQ(resp->instruments(index).nominal().units(),                  1000);
    ASSERT_EQ(resp->instruments(index).nominal().nano(),                   0);
    ASSERT_EQ(resp->instruments(index).initial_nominal().currency(),       "usd");
    ASSERT_EQ(resp->instruments(index).initial_nominal().units(),          1000);
    ASSERT_EQ(resp->instruments(index).initial_nominal().nano(),           0);
    ASSERT_EQ(resp->instruments(index).state_reg_date().seconds(),         0);
    ASSERT_EQ(resp->instruments(index).placement_date().seconds(),         1187222400);
    ASSERT_EQ(resp->instruments(index).placement_price().currency(),       "");
    ASSERT_EQ(resp->instruments(index).placement_price().units(),          0);
    ASSERT_EQ(resp->instruments(index).placement_price().nano(),           0);
    ASSERT_EQ(resp->instruments(index).aci_value().currency(),             "usd");
    ASSERT_EQ(resp->instruments(index).aci_value().units(),                33);
    ASSERT_EQ(resp->instruments(index).aci_value().nano(),                 808222000);
    ASSERT_EQ(resp->instruments(index).country_of_risk(),                  "RU");
    ASSERT_EQ(resp->instruments(index).country_of_risk_name(),             "\u0420\u043E\u0441\u0441\u0438\u0439\u0441\u043A\u0430\u044F \u0424\u0435\u0434\u0435\u0440\u0430\u0446\u0438\u044F"); // Russian Federation
    ASSERT_EQ(resp->instruments(index).sector(),                           "energy");
    ASSERT_EQ(resp->instruments(index).issue_kind(),                       "documentary");
    ASSERT_EQ(resp->instruments(index).issue_size(),                       1250000);
    ASSERT_EQ(resp->instruments(index).issue_size_plan(),                  1250000);
    // ASSERT_EQ(resp->instruments(index).trading_status(),                   tinkoff::SECURITY_TRADING_STATUS_NOT_AVAILABLE_FOR_TRADING);
    ASSERT_EQ(resp->instruments(index).otc_flag(),                         false);
    ASSERT_EQ(resp->instruments(index).buy_available_flag(),               false);
    ASSERT_EQ(resp->instruments(index).sell_available_flag(),              false);
    ASSERT_EQ(resp->instruments(index).floating_coupon_flag(),             false);
    ASSERT_EQ(resp->instruments(index).perpetual_flag(),                   false);
    ASSERT_EQ(resp->instruments(index).amortization_flag(),                false);
    ASSERT_EQ(resp->instruments(index).min_price_increment().units(),      0);
    ASSERT_EQ(resp->instruments(index).min_price_increment().nano(),       100000);
    ASSERT_EQ(resp->instruments(index).api_trade_available_flag(),         true);
    ASSERT_EQ(resp->instruments(index).uid(),                              BOND_UID);
    ASSERT_EQ(resp->instruments(index).real_exchange(),                    tinkoff::REAL_EXCHANGE_MOEX);
    ASSERT_EQ(resp->instruments(index).position_uid(),                     "fc20e8f8-cecd-4456-90e1-1333d98b234c");
    ASSERT_EQ(resp->instruments(index).asset_uid(),                        "398f5dc7-8b5b-4c8f-ad31-4214b3ee9a44");
    ASSERT_EQ(resp->instruments(index).for_iis_flag(),                     false);
    ASSERT_EQ(resp->instruments(index).for_qual_investor_flag(),           true);
    ASSERT_EQ(resp->instruments(index).weekend_flag(),                     false);
    ASSERT_EQ(resp->instruments(index).blocked_tca_flag(),                 false);
    ASSERT_EQ(resp->instruments(index).subordinated_flag(),                false);
    ASSERT_EQ(resp->instruments(index).liquidity_flag(),                   true);
    ASSERT_EQ(resp->instruments(index).first_1min_candle_date().seconds(), 1536323400);
    ASSERT_EQ(resp->instruments(index).first_1day_candle_date().seconds(), 1536303600);
    ASSERT_EQ(resp->instruments(index).risk_level(),                       tinkoff::RISK_LEVEL_LOW);
    ASSERT_EQ(resp->instruments(index).brand().logo_name(),                "RU0007661625.png");
    ASSERT_EQ(resp->instruments(index).brand().logo_base_color(),          "#117cbf");
    ASSERT_EQ(resp->instruments(index).brand().text_color(),               "#ffffff");
    ASSERT_EQ(resp->instruments(index).bond_type(),                        tinkoff::BOND_TYPE_UNSPECIFIED);
    ASSERT_EQ(resp->instruments(index).call_date().seconds(),              0);
    ASSERT_EQ(resp->instruments(index).dlong_client().units(),             0);
    ASSERT_EQ(resp->instruments(index).dlong_client().nano(),              0);
    ASSERT_EQ(resp->instruments(index).dshort_client().units(),            0);
    ASSERT_EQ(resp->instruments(index).dshort_client().nano(),             0);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_findCurrencies)
{
    const InSequence seq;

    grpc::ClientContext                                context;
    const tinkoff::InstrumentsRequest                  req;
    const std::shared_ptr<tinkoff::CurrenciesResponse> resp = std::make_shared<tinkoff::CurrenciesResponse>();

    context.set_credentials(creds);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->findCurrencies(instrumentsService, &context, req, resp.get());

    int index = -1;

    for (int i = 0; i < resp->instruments_size(); ++i)
    {
        if (resp->instruments(i).uid() == CURRENCY_UID)
        {
            index = i;

            break;
        }
    }

    qInfo() << resp->instruments_size();

    // clang-format off
    ASSERT_EQ(status.ok(),                                                 true);
    ASSERT_GE(resp->instruments_size(),                                    16);
    ASSERT_GE(index,                                                       0);
    ASSERT_EQ(resp->instruments(index).figi(),                             "BBG0013HGFT4");
    ASSERT_EQ(resp->instruments(index).ticker(),                           "USD000UTSTOM");
    ASSERT_EQ(resp->instruments(index).class_code(),                       "CETS");
    ASSERT_EQ(resp->instruments(index).isin(),                             "");
    ASSERT_EQ(resp->instruments(index).lot(),                              1000);
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
    ASSERT_EQ(resp->instruments(index).name(),                             "\u0414\u043E\u043B\u043B\u0430\u0440 \u0421\u0428\u0410"); // USA dollar
    ASSERT_EQ(resp->instruments(index).exchange(),                         "moex_close");
    ASSERT_EQ(resp->instruments(index).nominal().currency(),               "usd");
    ASSERT_EQ(resp->instruments(index).nominal().units(),                  1);
    ASSERT_EQ(resp->instruments(index).nominal().nano(),                   0);
    ASSERT_EQ(resp->instruments(index).country_of_risk(),                  "");
    ASSERT_EQ(resp->instruments(index).country_of_risk_name(),             "");
    // ASSERT_EQ(resp->instruments(index).trading_status(),                   tinkoff::SECURITY_TRADING_STATUS_NOT_AVAILABLE_FOR_TRADING);
    ASSERT_EQ(resp->instruments(index).otc_flag(),                         false);
    ASSERT_EQ(resp->instruments(index).buy_available_flag(),               false);
    ASSERT_EQ(resp->instruments(index).sell_available_flag(),              false);
    ASSERT_EQ(resp->instruments(index).iso_currency_name(),                "usd");
    ASSERT_EQ(resp->instruments(index).min_price_increment().units(),      0);
    ASSERT_EQ(resp->instruments(index).min_price_increment().nano(),       2500000);
    ASSERT_EQ(resp->instruments(index).api_trade_available_flag(),         true);
    ASSERT_EQ(resp->instruments(index).uid(),                              CURRENCY_UID);
    ASSERT_EQ(resp->instruments(index).real_exchange(),                    tinkoff::REAL_EXCHANGE_MOEX);
    ASSERT_EQ(resp->instruments(index).position_uid(),                     "6e97aa9b-50b6-4738-bce7-17313f2b2cc2");
    ASSERT_EQ(resp->instruments(index).for_iis_flag(),                     true);
    ASSERT_EQ(resp->instruments(index).for_qual_investor_flag(),           false);
    // ASSERT_EQ(resp->instruments(index).weekend_flag(),                     false);
    ASSERT_EQ(resp->instruments(index).blocked_tca_flag(),                 false);
    // ASSERT_EQ(resp->instruments(index).first_1min_candle_date().seconds(), 1520439360);
    // ASSERT_EQ(resp->instruments(index).first_1day_candle_date().seconds(), 958435200);
    ASSERT_EQ(resp->instruments(index).brand().logo_name(),                "USD1.png");
    ASSERT_EQ(resp->instruments(index).brand().logo_base_color(),          "#F6F8FA");
    ASSERT_EQ(resp->instruments(index).brand().text_color(),               "#000000");
    ASSERT_EQ(resp->instruments(index).dlong_client().units(),             0);
    ASSERT_EQ(resp->instruments(index).dlong_client().nano(),              0);
    ASSERT_EQ(resp->instruments(index).dshort_client().units(),            0);
    ASSERT_EQ(resp->instruments(index).dshort_client().nano(),             0);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_findEtfs)
{
    const InSequence seq;

    grpc::ClientContext                          context;
    const tinkoff::InstrumentsRequest            req;
    const std::shared_ptr<tinkoff::EtfsResponse> resp = std::make_shared<tinkoff::EtfsResponse>();

    context.set_credentials(creds);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->findEtfs(instrumentsService, &context, req, resp.get());

    int index = -1;

    for (int i = 0; i < resp->instruments_size(); ++i)
    {
        if (resp->instruments(i).uid() == ETF_UID)
        {
            index = i;

            break;
        }
    }

    // clang-format off
    ASSERT_EQ(status.ok(),                                                 true);
    ASSERT_GE(resp->instruments_size(),                                    277);
    ASSERT_GE(index,                                                       0);
    ASSERT_EQ(resp->instruments(index).figi(),                             "TCS00A100P44");
    ASSERT_EQ(resp->instruments(index).ticker(),                           "SBRB");
    ASSERT_EQ(resp->instruments(index).class_code(),                       "TQTF");
    ASSERT_EQ(resp->instruments(index).isin(),                             "RU000A100P44");
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
    ASSERT_EQ(resp->instruments(index).name(),                             "\u041F\u0435\u0440\u0432\u0430\u044F \u2013 \u0424\u043E\u043D\u0434 \u041A\u043E\u0440\u043F\u043E\u0440\u0430\u0442\u0438\u0432\u043D\u044B\u0435 \u043E\u0431\u043B\u0438\u0433\u0430\u0446\u0438\u0438"); // First - Corporate Bonds Fund
    ASSERT_EQ(resp->instruments(index).exchange(),                         "moex_etf");
    ASSERT_EQ(resp->instruments(index).fixed_commission().units(),         0);
    ASSERT_EQ(resp->instruments(index).fixed_commission().nano(),          0);
    ASSERT_EQ(resp->instruments(index).focus_type(),                       "equity");
    ASSERT_EQ(resp->instruments(index).released_date().seconds(),          1564012800);
    ASSERT_EQ(resp->instruments(index).num_shares().units(),               0);
    ASSERT_EQ(resp->instruments(index).num_shares().nano(),                0);
    ASSERT_EQ(resp->instruments(index).country_of_risk(),                  "RU");
    ASSERT_EQ(resp->instruments(index).country_of_risk_name(),             "\u0420\u043E\u0441\u0441\u0438\u0439\u0441\u043A\u0430\u044F \u0424\u0435\u0434\u0435\u0440\u0430\u0446\u0438\u044F"); // Russian Federation
    ASSERT_EQ(resp->instruments(index).sector(),                           "other");
    ASSERT_EQ(resp->instruments(index).rebalancing_freq(),                 "");
    // ASSERT_EQ(resp->instruments(index).trading_status(),                   tinkoff::SECURITY_TRADING_STATUS_NOT_AVAILABLE_FOR_TRADING);
    ASSERT_EQ(resp->instruments(index).otc_flag(),                         false);
    ASSERT_EQ(resp->instruments(index).buy_available_flag(),               true);
    ASSERT_EQ(resp->instruments(index).sell_available_flag(),              true);
    ASSERT_EQ(resp->instruments(index).min_price_increment().units(),      0);
    ASSERT_EQ(resp->instruments(index).min_price_increment().nano(),       1000000);
    ASSERT_EQ(resp->instruments(index).api_trade_available_flag(),         true);
    ASSERT_EQ(resp->instruments(index).uid(),                              ETF_UID);
    ASSERT_EQ(resp->instruments(index).real_exchange(),                    tinkoff::REAL_EXCHANGE_MOEX);
    ASSERT_EQ(resp->instruments(index).position_uid(),                     "3a7f880d-77ce-4bd9-9211-751a3955c772");
    ASSERT_EQ(resp->instruments(index).asset_uid(),                        "829b2317-0789-46da-9245-74f220bfe4fe");
    ASSERT_EQ(resp->instruments(index).instrument_exchange(),              tinkoff::INSTRUMENT_EXCHANGE_UNSPECIFIED);
    ASSERT_EQ(resp->instruments(index).for_iis_flag(),                     true);
    ASSERT_EQ(resp->instruments(index).for_qual_investor_flag(),           false);
    ASSERT_EQ(resp->instruments(index).weekend_flag(),                     false);
    ASSERT_EQ(resp->instruments(index).blocked_tca_flag(),                 false);
    ASSERT_EQ(resp->instruments(index).liquidity_flag(),                   true);
    ASSERT_EQ(resp->instruments(index).first_1min_candle_date().seconds(), 1567758840);
    ASSERT_EQ(resp->instruments(index).first_1day_candle_date().seconds(), 1567753200);
    ASSERT_EQ(resp->instruments(index).brand().logo_name(),                "logoam11.png");
    ASSERT_EQ(resp->instruments(index).brand().logo_base_color(),          "#007ba2");
    ASSERT_EQ(resp->instruments(index).brand().text_color(),               "#ffffff");
    ASSERT_EQ(resp->instruments(index).dlong_client().units(),             0);
    ASSERT_EQ(resp->instruments(index).dlong_client().nano(),              0);
    ASSERT_EQ(resp->instruments(index).dshort_client().units(),            0);
    ASSERT_EQ(resp->instruments(index).dshort_client().nano(),             0);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_findFutures)
{
    const InSequence seq;

    grpc::ClientContext                             context;
    const tinkoff::InstrumentsRequest               req;
    const std::shared_ptr<tinkoff::FuturesResponse> resp = std::make_shared<tinkoff::FuturesResponse>();

    context.set_credentials(creds);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->findFutures(instrumentsService, &context, req, resp.get());

    int index = -1;

    for (int i = 0; i < resp->instruments_size(); ++i)
    {
        if (resp->instruments(i).uid() == FUTURE_UID)
        {
            index = i;

            break;
        }
    }

    // clang-format off
    ASSERT_EQ(status.ok(),                                                   true);
    ASSERT_GE(resp->instruments_size(),                                      331);
    ASSERT_GE(index,                                                         0);
    ASSERT_EQ(resp->instruments(index).figi(),                               "FUTMMI092500");
    ASSERT_EQ(resp->instruments(index).ticker(),                             "MAU5");
    ASSERT_EQ(resp->instruments(index).class_code(),                         "SPBFUT");
    ASSERT_EQ(resp->instruments(index).lot(),                                1);
    ASSERT_EQ(resp->instruments(index).currency(),                           "rub");
    // ASSERT_EQ(resp->instruments(index).dlong().units(),                      0);
    // ASSERT_EQ(resp->instruments(index).dlong().nano(),                       350000000);
    // ASSERT_EQ(resp->instruments(index).dshort().units(),                     0);
    // ASSERT_EQ(resp->instruments(index).dshort().nano(),                      350000000);
    // ASSERT_EQ(resp->instruments(index).dlong_min().units(),                  0);
    // ASSERT_EQ(resp->instruments(index).dlong_min().nano(),                   130100000);
    // ASSERT_EQ(resp->instruments(index).dshort_min().units(),                 0);
    // ASSERT_EQ(resp->instruments(index).dshort_min().nano(),                  93200000);
    ASSERT_EQ(resp->instruments(index).short_enabled_flag(),                 true);
    ASSERT_EQ(resp->instruments(index).name(),                               "MMI-9.25 \u0418\u043D\u0434\u0435\u043A\u0441 \u041C\u0435\u0442\u0430\u043B\u043B\u043E\u0432 \u0438 \u0434\u043E\u0431\u044B\u0447\u0438"); // MMI-9.25 Metals and Mining Index
    ASSERT_EQ(resp->instruments(index).exchange(),                           "FORTS_EVENING");
    ASSERT_EQ(resp->instruments(index).first_trade_date().seconds(),         1741330800);
    ASSERT_EQ(resp->instruments(index).last_trade_date().seconds(),          1758211500);
    ASSERT_EQ(resp->instruments(index).futures_type(),                       "DELIVERY_TYPE_CASH_SETTLEMENT");
    ASSERT_EQ(resp->instruments(index).asset_type(),                         "TYPE_INDEX");
    ASSERT_EQ(resp->instruments(index).basic_asset(),                        "MOEXMM");
    ASSERT_EQ(resp->instruments(index).basic_asset_size().units(),           1);
    ASSERT_EQ(resp->instruments(index).basic_asset_size().nano(),            0);
    ASSERT_EQ(resp->instruments(index).country_of_risk(),                    "RU");
    ASSERT_EQ(resp->instruments(index).country_of_risk_name(),               "\u0420\u043E\u0441\u0441\u0438\u0439\u0441\u043A\u0430\u044F \u0424\u0435\u0434\u0435\u0440\u0430\u0446\u0438\u044F"); // Russian Federation
    ASSERT_EQ(resp->instruments(index).sector(),                             "SECTOR_MATERIALS");
    ASSERT_EQ(resp->instruments(index).expiration_date().seconds(),          1758153600);
    // ASSERT_EQ(resp->instruments(index).trading_status(),                     tinkoff::SECURITY_TRADING_STATUS_BREAK_IN_TRADING);
    ASSERT_EQ(resp->instruments(index).otc_flag(),                           false);
    ASSERT_EQ(resp->instruments(index).buy_available_flag(),                 true);
    ASSERT_EQ(resp->instruments(index).sell_available_flag(),                true);
    ASSERT_EQ(resp->instruments(index).min_price_increment().units(),        1);
    ASSERT_EQ(resp->instruments(index).min_price_increment().nano(),         0);
    ASSERT_EQ(resp->instruments(index).api_trade_available_flag(),           true);
    ASSERT_EQ(resp->instruments(index).uid(),                                FUTURE_UID);
    ASSERT_EQ(resp->instruments(index).real_exchange(),                      tinkoff::REAL_EXCHANGE_MOEX);
    ASSERT_EQ(resp->instruments(index).position_uid(),                       "3ce9e351-a650-4b26-bb88-6658b9eee7ba");
    ASSERT_EQ(resp->instruments(index).basic_asset_position_uid(),           "");
    ASSERT_EQ(resp->instruments(index).for_iis_flag(),                       true);
    ASSERT_EQ(resp->instruments(index).for_qual_investor_flag(),             false);
    ASSERT_EQ(resp->instruments(index).weekend_flag(),                       false);
    ASSERT_EQ(resp->instruments(index).blocked_tca_flag(),                   false);
    ASSERT_EQ(resp->instruments(index).first_1min_candle_date().seconds(),   1743487200);
    ASSERT_EQ(resp->instruments(index).first_1day_candle_date().seconds(),   1743465600);
    // ASSERT_EQ(resp->instruments(index).initial_margin_on_buy().currency(),   "rub");
    // ASSERT_EQ(resp->instruments(index).initial_margin_on_buy().units(),      1401);
    // ASSERT_EQ(resp->instruments(index).initial_margin_on_buy().nano(),       10000000);
    // ASSERT_EQ(resp->instruments(index).initial_margin_on_sell().currency(),  "rub");
    // ASSERT_EQ(resp->instruments(index).initial_margin_on_sell().units(),     1466);
    // ASSERT_EQ(resp->instruments(index).initial_margin_on_sell().nano(),      530000000);
    // ASSERT_EQ(resp->instruments(index).min_price_increment_amount().units(), 1);
    // ASSERT_EQ(resp->instruments(index).min_price_increment_amount().nano(),  0);
    ASSERT_EQ(resp->instruments(index).brand().logo_name(),                  "MOEXMM.png");
    ASSERT_EQ(resp->instruments(index).brand().logo_base_color(),            "#b50000");
    ASSERT_EQ(resp->instruments(index).brand().text_color(),                 "#ffffff");
    // ASSERT_EQ(resp->instruments(index).dlong_client().units(),               0);
    // ASSERT_EQ(resp->instruments(index).dlong_client().nano(),                350000000);
    // ASSERT_EQ(resp->instruments(index).dshort_client().units(),              0);
    // ASSERT_EQ(resp->instruments(index).dshort_client().nano(),               350000000);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_getCandles)
{
    const InSequence seq;

    grpc::ClientContext                                context;
    tinkoff::GetCandlesRequest                         req;
    const std::shared_ptr<tinkoff::GetCandlesResponse> resp = std::make_shared<tinkoff::GetCandlesResponse>();

    context.set_credentials(creds);

    google::protobuf::Timestamp* fromTimestamp = new google::protobuf::Timestamp(); // req will take ownership
    google::protobuf::Timestamp* toTimestamp   = new google::protobuf::Timestamp(); // req will take ownership

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

    const grpc::Status status = client->getCandles(marketDataService, &context, req, resp.get());

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

    grpc::ClientContext                                  context;
    tinkoff::GetOrderBookRequest                         req;
    const std::shared_ptr<tinkoff::GetOrderBookResponse> resp = std::make_shared<tinkoff::GetOrderBookResponse>();

    context.set_credentials(creds);

    req.set_instrument_id(SPBE_UID);
    req.set_depth(50);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->getOrderBook(marketDataService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),            true);
    ASSERT_EQ(resp->depth(),          50);
    ASSERT_EQ(resp->instrument_uid(), SPBE_UID);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_getPortfolio)
{
    const InSequence seq;

    grpc::ClientContext                               context;
    tinkoff::PortfolioRequest                         req;
    const std::shared_ptr<tinkoff::PortfolioResponse> resp = std::make_shared<tinkoff::PortfolioResponse>();

    context.set_credentials(creds);

    req.set_account_id("d1843f24-0864-4666-8608-d5d16822fbae");

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->getPortfolio(operationsService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),                             true);
    ASSERT_EQ(resp->account_id(),                      "d1843f24-0864-4666-8608-d5d16822fbae");
    ASSERT_NE(resp->total_amount_currencies().units(), 0);
    ASSERT_NE(resp->total_amount_portfolio().units(),  0);
    ASSERT_GT(resp->positions_size(),                  0);
    ASSERT_EQ(resp->positions(0).figi(),               "RUB000UTSTOM");
    ASSERT_EQ(resp->positions(0).instrument_type(),    "currency");
    ASSERT_NE(resp->positions(0).quantity().units(),   0);
    ASSERT_NE(resp->positions(0).position_uid(),       "");
    ASSERT_EQ(resp->positions(0).instrument_uid(),     "a92e2e25-a698-45cc-a781-167cf465257c");
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_getPositions)
{
    const InSequence seq;

    grpc::ClientContext                               context;
    tinkoff::PositionsRequest                         req;
    const std::shared_ptr<tinkoff::PositionsResponse> resp = std::make_shared<tinkoff::PositionsResponse>();

    context.set_credentials(creds);

    req.set_account_id("d1843f24-0864-4666-8608-d5d16822fbae");

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->getPositions(operationsService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),            true);
    ASSERT_GT(resp->money_size(),     0);
    ASSERT_NE(resp->money(0).units(), 0);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_getOperations)
{
    const InSequence seq;

    grpc::ClientContext                                           context;
    tinkoff::GetOperationsByCursorRequest                         req;
    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> resp =
        std::make_shared<tinkoff::GetOperationsByCursorResponse>();

    context.set_credentials(creds);

    req.set_account_id("d1843f24-0864-4666-8608-d5d16822fbae");
    req.set_limit(2);
    req.set_state(tinkoff::OPERATION_STATE_EXECUTED);
    req.set_without_trades(true);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->getOperations(operationsService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),                        true);
    ASSERT_GT(resp->items_size(),                 0);
    ASSERT_NE(resp->items(0).cursor(),            "");
    ASSERT_EQ(resp->items(0).broker_account_id(), "d1843f24-0864-4666-8608-d5d16822fbae");
    ASSERT_NE(resp->items(0).id(),                "");
    ASSERT_GT(resp->items(0).date().seconds(),    1704056400);
    ASSERT_NE(resp->items(0).description(),       "");
    ASSERT_EQ(resp->items(0).state(),             tinkoff::OPERATION_STATE_EXECUTED);
    ASSERT_NE(resp->items(0).instrument_uid(),    "");
    ASSERT_NE(resp->items(0).figi(),              "");
    ASSERT_NE(resp->items(0).instrument_type(),   "");
    ASSERT_NE(resp->items(0).position_uid(),      "");
    ASSERT_NE(resp->items(0).payment().units(),   0);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_getMaxLots)
{
    const InSequence seq;

    grpc::ClientContext                                context;
    tinkoff::GetMaxLotsRequest                         req;
    const std::shared_ptr<tinkoff::GetMaxLotsResponse> resp = std::make_shared<tinkoff::GetMaxLotsResponse>();

    context.set_credentials(creds);

    req.set_account_id("d1843f24-0864-4666-8608-d5d16822fbae");
    req.set_instrument_id(SPBE_UID);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status = client->getMaxLots(ordersService, &context, req, resp.get());

    // clang-format off
    ASSERT_EQ(status.ok(),                                   true);
    ASSERT_EQ(resp->currency(),                              "rub");
    ASSERT_GT(resp->buy_limits().buy_money_amount().units(), 0);
    ASSERT_GT(resp->buy_limits().buy_max_lots(),             0);
    ASSERT_GE(resp->sell_limits().sell_max_lots(),           0);
    // clang-format on
}

TEST_F(Test_RawGrpcClient, Test_postOrder_and_getOrderState_and_cancelOrder)
{
    const InSequence seq;

    grpc::ClientContext                               context1;
    tinkoff::PostOrderRequest                         req1;
    const std::shared_ptr<tinkoff::PostOrderResponse> resp1 = std::make_shared<tinkoff::PostOrderResponse>();

    context1.set_credentials(creds);

    tinkoff::Quotation* tinkoffPrice = new tinkoff::Quotation(); // req1 will take ownership

    tinkoffPrice->set_units(1);
    tinkoffPrice->set_nano(0);

    req1.set_account_id("d1843f24-0864-4666-8608-d5d16822fbae");
    req1.set_instrument_id(SPBE_UID);
    req1.set_direction(tinkoff::ORDER_DIRECTION_BUY);
    req1.set_quantity(1);
    req1.set_allocated_price(tinkoffPrice);
    req1.set_order_type(tinkoff::ORDER_TYPE_LIMIT);
    req1.set_time_in_force(tinkoff::TIME_IN_FORCE_DAY);
    req1.set_price_type(tinkoff::PRICE_TYPE_CURRENCY);

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status1 = client->postOrder(ordersService, &context1, req1, resp1.get());

    // clang-format off
    ASSERT_EQ(status1.ok(),                                       true);
    ASSERT_NE(resp1->order_id(),                                  "");
    ASSERT_EQ(resp1->execution_report_status(),                   tinkoff::EXECUTION_REPORT_STATUS_NEW);
    ASSERT_EQ(resp1->lots_requested(),                            1);
    ASSERT_EQ(resp1->lots_executed(),                             0);
    ASSERT_EQ(resp1->initial_order_price().currency(),            "rub");
    ASSERT_EQ(resp1->initial_order_price().units(),               1);
    ASSERT_EQ(resp1->initial_order_price().nano(),                0);
    ASSERT_EQ(resp1->executed_order_price().currency(),           "rub");
    ASSERT_EQ(resp1->executed_order_price().units(),              0);
    ASSERT_EQ(resp1->executed_order_price().nano(),               0);
    ASSERT_EQ(resp1->total_order_amount().currency(),             "rub");
    ASSERT_EQ(resp1->total_order_amount().units(),                1);
    ASSERT_EQ(resp1->total_order_amount().nano(),                 0);
    ASSERT_EQ(resp1->initial_commission().currency(),             "rub");
    ASSERT_EQ(resp1->initial_commission().units(),                0);
    ASSERT_EQ(resp1->initial_commission().nano(),                 500000);
    ASSERT_EQ(resp1->executed_commission().currency(),            "rub");
    ASSERT_EQ(resp1->executed_commission().units(),               0);
    ASSERT_EQ(resp1->executed_commission().nano(),                0);
    ASSERT_EQ(resp1->figi(),                                      "TCS60A0JQ9P9");
    ASSERT_EQ(resp1->direction(),                                 tinkoff::ORDER_DIRECTION_BUY);
    ASSERT_EQ(resp1->initial_security_price().currency(),         "rub");
    ASSERT_EQ(resp1->initial_security_price().units(),            1);
    ASSERT_EQ(resp1->initial_security_price().nano(),             0);
    ASSERT_EQ(resp1->order_type(),                                tinkoff::ORDER_TYPE_LIMIT);
    ASSERT_EQ(resp1->message(),                                   "");
    ASSERT_EQ(resp1->initial_order_price_pt().units(),            0);
    ASSERT_EQ(resp1->initial_order_price_pt().nano(),             0);
    ASSERT_EQ(resp1->instrument_uid(),                            SPBE_UID);
    ASSERT_EQ(resp1->order_request_id(),                          "");
    ASSERT_NE(resp1->response_metadata().tracking_id(),           "");
    ASSERT_GT(resp1->response_metadata().server_time().seconds(), 1704056400);
    // clang-format on

    grpc::ClientContext                        context2;
    tinkoff::GetOrderStateRequest              req2;
    const std::shared_ptr<tinkoff::OrderState> resp2 = std::make_shared<tinkoff::OrderState>();

    context2.set_credentials(creds);

    req2.set_account_id("d1843f24-0864-4666-8608-d5d16822fbae");
    req2.set_order_id(resp1->order_id());
    req2.set_price_type(tinkoff::PRICE_TYPE_CURRENCY);
    req2.set_order_id_type(tinkoff::ORDER_ID_TYPE_EXCHANGE);

    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status2 = client->getOrderState(ordersService, &context2, req2, resp2.get());

    // clang-format off
    ASSERT_EQ(status2.ok(),                                       true);
    ASSERT_NE(resp2->order_id(),                                  "");
    ASSERT_EQ(resp2->execution_report_status(),                   tinkoff::EXECUTION_REPORT_STATUS_NEW);
    ASSERT_EQ(resp2->lots_requested(),                            1);
    ASSERT_EQ(resp2->lots_executed(),                             0);
    ASSERT_EQ(resp2->initial_order_price().currency(),            "rub");
    ASSERT_EQ(resp2->initial_order_price().units(),               1);
    ASSERT_EQ(resp2->initial_order_price().nano(),                0);
    ASSERT_EQ(resp2->executed_order_price().currency(),           "rub");
    ASSERT_EQ(resp2->executed_order_price().units(),              0);
    ASSERT_EQ(resp2->executed_order_price().nano(),               0);
    ASSERT_EQ(resp2->total_order_amount().currency(),             "rub");
    ASSERT_EQ(resp2->total_order_amount().units(),                1);
    ASSERT_EQ(resp2->total_order_amount().nano(),                 0);
    ASSERT_EQ(resp2->average_position_price().currency(),         "rub");
    ASSERT_EQ(resp2->average_position_price().units(),            1);
    ASSERT_EQ(resp2->average_position_price().nano(),             0);
    ASSERT_EQ(resp2->initial_commission().currency(),             "rub");
    ASSERT_EQ(resp2->initial_commission().units(),                0);
    ASSERT_EQ(resp2->initial_commission().nano(),                 500000);
    ASSERT_EQ(resp2->executed_commission().currency(),            "rub");
    ASSERT_EQ(resp2->executed_commission().units(),               0);
    ASSERT_EQ(resp2->executed_commission().nano(),                0);
    ASSERT_EQ(resp2->figi(),                                      "TCS60A0JQ9P9");
    ASSERT_EQ(resp2->direction(),                                 tinkoff::ORDER_DIRECTION_BUY);
    ASSERT_EQ(resp2->initial_security_price().currency(),         "rub");
    ASSERT_EQ(resp2->initial_security_price().units(),            1);
    ASSERT_EQ(resp2->initial_security_price().nano(),             0);
    ASSERT_EQ(resp2->stages_size(),                               0);
    ASSERT_EQ(resp2->service_commission().currency(),             "rub");
    ASSERT_EQ(resp2->service_commission().units(),                0);
    ASSERT_EQ(resp2->service_commission().nano(),                 0);
    ASSERT_EQ(resp2->currency(),                                  "rub");
    ASSERT_EQ(resp2->order_type(),                                tinkoff::ORDER_TYPE_LIMIT);
    ASSERT_GT(resp2->order_date().seconds(),                      1704056400);
    ASSERT_EQ(resp2->instrument_uid(),                            SPBE_UID);
    ASSERT_EQ(resp2->order_request_id(),                          "");
    // clang-format on

    grpc::ClientContext                                 context3;
    tinkoff::CancelOrderRequest                         req3;
    const std::shared_ptr<tinkoff::CancelOrderResponse> resp3 = std::make_shared<tinkoff::CancelOrderResponse>();

    context3.set_credentials(creds);

    req3.set_account_id("d1843f24-0864-4666-8608-d5d16822fbae");
    req3.set_order_id(resp1->order_id());
    req3.set_order_id_type(tinkoff::ORDER_ID_TYPE_EXCHANGE);

    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    const grpc::Status status3 = client->cancelOrder(ordersService, &context3, req3, resp3.get());

    // clang-format off
    ASSERT_EQ(status3.ok(),                                       true);
    ASSERT_GT(resp3->time().seconds(),                            1704056400);
    ASSERT_NE(resp3->response_metadata().tracking_id(),           "");
    ASSERT_GT(resp3->response_metadata().server_time().seconds(), 1704056400);
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

    ASSERT_EQ(client->closeWriteMarketDataStream(marketDataStream), true);
    marketDataStream->context.TryCancel();
    ASSERT_EQ(client->finishMarketDataStream(marketDataStream).error_code(), grpc::StatusCode::CANCELLED);
}

TEST_F(Test_RawGrpcClient, Test_PortfolioStream)
{
    const InSequence seq;

    tinkoff::PortfolioStreamRequest req;
    req.add_accounts("d1843f24-0864-4666-8608-d5d16822fbae");

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    std::shared_ptr<PortfolioStream> portfolioStream(new PortfolioStream());

    portfolioStream->context.set_credentials(creds);
    portfolioStream->stream = client->createPortfolioStream(operationsStreamService, &portfolioStream->context, req);

    ASSERT_NE(portfolioStream->stream, nullptr);

    portfolioStream->context.TryCancel();

    tinkoff::PortfolioStreamResponse resp;
    ASSERT_EQ(client->readPortfolioStream(portfolioStream, &resp), false);

    ASSERT_EQ(client->finishPortfolioStream(portfolioStream).error_code(), grpc::StatusCode::CANCELLED);
}

TEST_F(Test_RawGrpcClient, Test_PositionsStream)
{
    const InSequence seq;

    tinkoff::PositionsStreamRequest req;
    req.add_accounts("d1843f24-0864-4666-8608-d5d16822fbae");

    QString token = SANDBOX_TOKEN;
    EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(token));

    std::shared_ptr<PositionsStream> positionsStream(new PositionsStream());

    positionsStream->context.set_credentials(creds);
    positionsStream->stream = client->createPositionsStream(operationsStreamService, &positionsStream->context, req);

    ASSERT_NE(positionsStream->stream, nullptr);

    positionsStream->context.TryCancel();

    tinkoff::PositionsStreamResponse resp;
    ASSERT_EQ(client->readPositionsStream(positionsStream, &resp), false);

    ASSERT_EQ(client->finishPositionsStream(positionsStream).error_code(), grpc::StatusCode::CANCELLED);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
