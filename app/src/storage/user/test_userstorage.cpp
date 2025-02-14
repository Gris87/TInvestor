#include "src/storage/account/accountstorage.h"

#include <gtest/gtest.h>

#include "src/db/account/iaccountdatabase_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_AccountStorage, Test_constructor_and_destructor)
{
    AccountStorage storage;
}

TEST(Test_AccountStorage, Test_readFromDatabase_and_getToken)
{
    StrictMock<AccountDatabaseMock> accountDatabaseMock;

    AccountStorage storage;

    Account account;
    account.token = "someToken";

    EXPECT_CALL(accountDatabaseMock, readAccountInfo()).WillOnce(Return(account));

    storage.readFromDatabase(&accountDatabaseMock);

    ASSERT_EQ(storage.getToken(), "someToken");
}
