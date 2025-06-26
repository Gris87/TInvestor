#include "src/storage/user/userstorage.h"

#include <gtest/gtest.h>

#include "src/db/user/iuserdatabase_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_UserStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userDatabaseMock = new StrictMock<UserDatabaseMock>();

        storage = new UserStorage(userDatabaseMock);
    }

    void TearDown() override
    {
        delete storage;
        delete userDatabaseMock;
    }

    UserStorage*                  storage;
    StrictMock<UserDatabaseMock>* userDatabaseMock;
};



TEST_F(Test_UserStorage, Test_constructor_and_destructor)
{
}

TEST_F(Test_UserStorage, Test_readFromDatabase_and_getToken_and_isQualified_and_getCommission_and_getAccounts)
{
    const InSequence seq;

    User user;
    user.token      = "someToken";
    user.qualified  = true;
    user.commission = 0.99f;

    Accounts accounts;

    Account account1;
    Account account2;

    account1.index = 0;
    account1.id    = "aaaa-aaaa";
    account1.name  = "Bill";

    account2.index = 1;
    account2.id    = "bbb-bbb-bbb";
    account2.name  = "John";

    accounts[account1.hash()] = account1;
    accounts[account2.hash()] = account2;

    EXPECT_CALL(*userDatabaseMock, readUserInfo()).WillOnce(Return(user));
    EXPECT_CALL(*userDatabaseMock, readAccounts()).WillOnce(Return(accounts));

    storage->readFromDatabase();

    // clang-format off
    ASSERT_EQ(storage->getToken(),        "someToken");
    ASSERT_EQ(storage->isQualified(),     true);
    ASSERT_NEAR(storage->getCommission(), 0.99f, 0.0001f);
    ASSERT_EQ(storage->getAccounts(),     accounts);
    // clang-format on
}

TEST_F(Test_UserStorage, Test_lock_unlock)
{
    storage->writeLock();
    storage->writeUnlock();
    storage->readLock();
    storage->readUnlock();
}

TEST_F(Test_UserStorage, Test_setToken)
{
    const InSequence seq;

    User user;
    user.token = "someToken";

    Accounts accounts;

    Account account1;
    Account account2;

    account1.index = 0;
    account1.id    = "aaaa-aaaa";
    account1.name  = "Bill";

    account2.index = 1;
    account2.id    = "bbb-bbb-bbb";
    account2.name  = "John";

    accounts[account1.hash()] = account1;
    accounts[account2.hash()] = account2;

    EXPECT_CALL(*userDatabaseMock, readUserInfo()).WillOnce(Return(user));
    EXPECT_CALL(*userDatabaseMock, readAccounts()).WillOnce(Return(accounts));

    storage->readFromDatabase();

    ASSERT_EQ(storage->getToken(), "someToken");
    ASSERT_EQ(storage->getAccounts(), accounts);

    EXPECT_CALL(*userDatabaseMock, writeToken(QString("BlahToken")));

    storage->setToken("BlahToken");
    ASSERT_EQ(storage->getToken(), "BlahToken");
}

TEST_F(Test_UserStorage, Test_setUserInfo)
{
    const InSequence seq;

    User user;
    user.tariff = "premium";

    EXPECT_CALL(*userDatabaseMock, writeUserInfo(_));

    storage->setUserInfo(user);
}

TEST_F(Test_UserStorage, Test_setAccounts)
{
    const InSequence seq;

    User user;
    user.token = "someToken";

    Accounts accounts;

    Account account1;
    Account account2;

    account1.index = 0;
    account1.id    = "aaaa-aaaa";
    account1.name  = "Bill";

    account2.index = 1;
    account2.id    = "bbb-bbb-bbb";
    account2.name  = "John";

    accounts[account1.hash()] = account1;
    accounts[account2.hash()] = account2;

    EXPECT_CALL(*userDatabaseMock, readUserInfo()).WillOnce(Return(user));
    EXPECT_CALL(*userDatabaseMock, readAccounts()).WillOnce(Return(accounts));

    storage->readFromDatabase();

    ASSERT_EQ(storage->getToken(), "someToken");
    ASSERT_EQ(storage->getAccounts(), accounts);

    accounts[account2.hash()].name = "Vasya";

    EXPECT_CALL(*userDatabaseMock, writeAccounts(accounts));

    storage->setAccounts(accounts);
    ASSERT_EQ(storage->getAccounts(), accounts);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
