#include "src/grpc/rawgrpcclient.h"

#include <QDebug>
#include <grpcpp/grpcpp.h>
#include <gtest/gtest.h>

#include "src/domain/user/user.h"
#include "src/grpc/investapiuthenticator.h"
#include "src/storage/user/iuserstorage_mock.h"



#define ADDRESS "sandbox-invest-public-api.tinkoff.ru:443"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_RawGrpcClient : public ::testing::Test
{
protected:
    void SetUp()
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

    void TearDown()
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
    InSequence seq;

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
    InSequence seq;

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
