#include "src/threads/userupdate/userupdatethread.h"

#include <gtest/gtest.h>

#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/user/iuserstorage_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_UserUpdateThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();
        grpcClientMock  = new StrictMock<GrpcClientMock>();

        thread = new UserUpdateThread(userStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete userStorageMock;
        delete grpcClientMock;
    }

    UserUpdateThread*            thread;
    StrictMock<UserStorageMock>* userStorageMock;
    StrictMock<GrpcClientMock>*  grpcClientMock;
};



TEST_F(Test_UserUpdateThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_UserUpdateThread, Test_run)
{
    const InSequence seq;

    User     user;
    Accounts accounts;
    Account  account1;
    Account  account2;

    user.qualified = true;
    user.tariff    = "premium";
    user.qualifiedForWorkWith << "blah" << "disintegration gun";

    account1.index = 0;
    account1.id    = "aaaaa";
    account1.name  = "For the black day";

    account2.index = 1;
    account2.id    = "bbbbb";
    account2.name  = "Everyday party";

    accounts[account1.hash()] = account1;
    accounts[account2.hash()] = account2;

    const std::shared_ptr<tinkoff::GetInfoResponse> getInfoResponse(new tinkoff::GetInfoResponse());

    getInfoResponse->set_qual_status(true);
    getInfoResponse->add_qualified_for_work_with("blah");
    getInfoResponse->add_qualified_for_work_with("disintegration gun");
    getInfoResponse->set_tariff("premium");

    const std::shared_ptr<tinkoff::GetAccountsResponse> getAccountsResponse(new tinkoff::GetAccountsResponse());

    tinkoff::Account* tinkoffAccount1 = getAccountsResponse->add_accounts();
    tinkoff::Account* tinkoffAccount2 = getAccountsResponse->add_accounts();

    tinkoffAccount1->set_type(tinkoff::ACCOUNT_TYPE_TINKOFF);
    tinkoffAccount1->set_status(tinkoff::ACCOUNT_STATUS_OPEN);
    tinkoffAccount1->set_access_level(tinkoff::ACCOUNT_ACCESS_LEVEL_FULL_ACCESS);
    tinkoffAccount1->set_id("aaaaa");
    tinkoffAccount1->set_name("For the black day");

    tinkoffAccount2->set_type(tinkoff::ACCOUNT_TYPE_TINKOFF);
    tinkoffAccount2->set_status(tinkoff::ACCOUNT_STATUS_OPEN);
    tinkoffAccount2->set_access_level(tinkoff::ACCOUNT_ACCESS_LEVEL_FULL_ACCESS);
    tinkoffAccount2->set_id("bbbbb");
    tinkoffAccount2->set_name("Everyday party");

    EXPECT_CALL(*grpcClientMock, getUserInfo(QThread::currentThread())).WillOnce(Return(getInfoResponse));
    EXPECT_CALL(*grpcClientMock, getAccounts(QThread::currentThread())).WillOnce(Return(getAccountsResponse));
    EXPECT_CALL(*userStorageMock, writeLock());
    EXPECT_CALL(*userStorageMock, setUserInfo(user));
    EXPECT_CALL(*userStorageMock, setAccounts(accounts));
    EXPECT_CALL(*userStorageMock, writeUnlock());

    thread->run();
}

TEST_F(Test_UserUpdateThread, Test_terminateThread)
{
    thread->terminateThread();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
