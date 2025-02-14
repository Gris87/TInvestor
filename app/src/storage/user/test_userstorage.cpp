#include "src/storage/user/userstorage.h"

#include <gtest/gtest.h>

#include "src/db/user/iuserdatabase_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_UserStorage, Test_constructor_and_destructor)
{
    UserStorage storage;
}

TEST(Test_UserStorage, Test_readFromDatabase_and_getToken)
{
    StrictMock<UserDatabaseMock> userDatabaseMock;

    UserStorage storage;

    User user;
    user.token = "someToken";

    EXPECT_CALL(userDatabaseMock, readUserInfo()).WillOnce(Return(user));

    storage.readFromDatabase(&userDatabaseMock);

    ASSERT_EQ(storage.getToken(), "someToken");
}
