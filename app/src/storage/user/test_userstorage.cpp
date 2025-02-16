#include "src/storage/user/userstorage.h"

#include <gtest/gtest.h>

#include "src/db/user/iuserdatabase_mock.h"



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

TEST_F(Test_UserStorage, Test_readFromDatabase_and_getToken)
{
    User user;
    user.token = "someToken";

    EXPECT_CALL(*userDatabaseMock, readUserInfo()).WillOnce(Return(user));

    storage->readFromDatabase();

    ASSERT_EQ(storage->getToken(), "someToken");
}

TEST_F(Test_UserStorage, Test_setToken)
{
    User user;
    user.token = "someToken";

    EXPECT_CALL(*userDatabaseMock, readUserInfo()).WillOnce(Return(user));

    storage->readFromDatabase();

    ASSERT_EQ(storage->getToken(), "someToken");

    EXPECT_CALL(*userDatabaseMock, writeToken(QString("BlahToken")));

    storage->setToken("BlahToken");
    ASSERT_EQ(storage->getToken(), "BlahToken");
}
