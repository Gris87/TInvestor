#include "src/db/user/userdatabase.h"

#include <gtest/gtest.h>



class Test_UserDatabase : public ::testing::Test
{
protected:
    void SetUp()
    {
        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db/user").removeRecursively();

        database = new UserDatabase();
    }

    void TearDown()
    {
        delete database;

        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db/user").removeRecursively();
    }

    UserDatabase* database;
};



TEST_F(Test_UserDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_UserDatabase, Test_readUserInfo)
{
    User user = database->readUserInfo();

    // clang-format off
    ASSERT_EQ(user.token,                "");
    ASSERT_EQ(user.qualified,            false);
    ASSERT_EQ(user.qualifiedForWorkWith, QStringList());
    ASSERT_EQ(user.tariff,               "fees");
    ASSERT_NEAR(user.commission,         0.3f, 0.001f);
    // clang-format on
}

TEST_F(Test_UserDatabase, Test_writeToken)
{
    User user = database->readUserInfo();

    // clang-format off
    ASSERT_EQ(user.token,                "");
    ASSERT_EQ(user.qualified,            false);
    ASSERT_EQ(user.qualifiedForWorkWith, QStringList());
    ASSERT_EQ(user.tariff,               "fees");
    ASSERT_NEAR(user.commission,         0.3f, 0.001f);
    // clang-format on

    database->writeToken("SomeToken");
    user = database->readUserInfo();

    // clang-format off
    ASSERT_EQ(user.token,                "SomeToken");
    ASSERT_EQ(user.qualified,            false);
    ASSERT_EQ(user.qualifiedForWorkWith, QStringList());
    ASSERT_EQ(user.tariff,               "fees");
    ASSERT_NEAR(user.commission,         0.3f, 0.001f);
    // clang-format on
}

TEST_F(Test_UserDatabase, Test_writeUserInfo)
{
    User user = database->readUserInfo();

    // clang-format off
    ASSERT_EQ(user.token,                "");
    ASSERT_EQ(user.qualified,            false);
    ASSERT_EQ(user.qualifiedForWorkWith, QStringList());
    ASSERT_EQ(user.tariff,               "fees");
    ASSERT_NEAR(user.commission,         0.3f, 0.001f);
    // clang-format on

    user.qualified            = true;
    user.qualifiedForWorkWith = QStringList() << "god_mode" << "guest_killer";
    user.tariff               = "premium";

    database->writeUserInfo(user);
    user = database->readUserInfo();

    // clang-format off
    ASSERT_EQ(user.token,                "");
    ASSERT_EQ(user.qualified,            true);
    ASSERT_EQ(user.qualifiedForWorkWith, QStringList() << "god_mode" << "guest_killer");
    ASSERT_EQ(user.tariff,               "premium");
    ASSERT_NEAR(user.commission,         0.04f, 0.001f);
    // clang-format on
}
