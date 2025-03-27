#include "src/storage/user/userstorage.h"

#include <gtest/gtest.h>

#include "src/db/user/iuserdatabase_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_UserStorage : public ::testing::Test
{
protected:
    void SetUp()
    {
        userDatabaseMock = new StrictMock<UserDatabaseMock>();

        storage = new UserStorage(userDatabaseMock);
    }

    void TearDown()
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
    InSequence seq;

    User user;
    user.token      = "someToken";
    user.qualified  = true;
    user.commission = 0.99f;

    Account account1;
    Account account2;

    account1.setId("aaaa-aaaa");
    account1.name = "Bill";

    account2.setId("bbb-bbb-bbb");
    account2.name = "John";

    QList<Account> accounts;
    accounts << account1 << account2;

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

TEST_F(Test_UserStorage, Test_getMutex)
{
    ASSERT_NE(storage->getMutex(), nullptr);
}

TEST_F(Test_UserStorage, Test_setToken)
{
    InSequence seq;

    User user;
    user.token = "someToken";

    Account account1;
    Account account2;

    account1.setId("aaaa-aaaa");
    account1.name = "Bill";

    account2.setId("bbb-bbb-bbb");
    account2.name = "John";

    QList<Account> accounts;
    accounts << account1 << account2;

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
    InSequence seq;

    User user;
    user.tariff = "premium";

    EXPECT_CALL(*userDatabaseMock, writeUserInfo(_));

    storage->setUserInfo(user);
}

TEST_F(Test_UserStorage, Test_setAccounts)
{
    InSequence seq;

    User user;
    user.token = "someToken";

    Account account1;
    Account account2;

    account1.setId("aaaa-aaaa");
    account1.name = "Bill";

    account2.setId("bbb-bbb-bbb");
    account2.name = "John";

    QList<Account> accounts;
    accounts << account1 << account2;

    EXPECT_CALL(*userDatabaseMock, readUserInfo()).WillOnce(Return(user));
    EXPECT_CALL(*userDatabaseMock, readAccounts()).WillOnce(Return(accounts));

    storage->readFromDatabase();

    ASSERT_EQ(storage->getToken(), "someToken");
    ASSERT_EQ(storage->getAccounts(), accounts);

    accounts[0].setId("cccccccccccc");

    EXPECT_CALL(*userDatabaseMock, writeAccounts(accounts));

    storage->setAccounts(accounts);
    ASSERT_EQ(storage->getAccounts(), accounts);
}
