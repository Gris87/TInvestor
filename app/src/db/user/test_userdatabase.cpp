#include "src/db/user/userdatabase.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
class Test_UserDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/user").removeRecursively();

        database = new UserDatabase();
    }

    void TearDown() override
    {
        delete database;

        const QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/user").removeRecursively();
    }

    UserDatabase* database;
};



TEST_F(Test_UserDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_UserDatabase, Test_readUserInfo)
{
    const User user = database->readUserInfo();

    // clang-format off
    ASSERT_EQ(user.token,                "");
    ASSERT_EQ(user.qualified,            false);
    ASSERT_EQ(user.qualifiedForWorkWith, QStringList());
    ASSERT_EQ(user.tariff,               "fees");
    ASSERT_NEAR(user.commission,         0.3f, 0.0001f);
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
    ASSERT_NEAR(user.commission,         0.3f, 0.0001f);
    // clang-format on

    database->writeToken("SomeToken");
    user = database->readUserInfo();

    // clang-format off
    ASSERT_EQ(user.token,                "SomeToken");
    ASSERT_EQ(user.qualified,            false);
    ASSERT_EQ(user.qualifiedForWorkWith, QStringList());
    ASSERT_EQ(user.tariff,               "fees");
    ASSERT_NEAR(user.commission,         0.3f, 0.0001f);
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
    ASSERT_NEAR(user.commission,         0.3f, 0.0001f);
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
    ASSERT_NEAR(user.commission,         0.04f, 0.0001f);
    // clang-format on
}

TEST_F(Test_UserDatabase, Test_readAccounts)
{
    const Accounts accounts = database->readAccounts();

    ASSERT_EQ(accounts.size(), 0);
}

TEST_F(Test_UserDatabase, Test_writeAccounts)
{
    Accounts accounts = database->readAccounts();

    ASSERT_EQ(accounts.size(), 0);

    Account account1;
    Account account2;

    account1.index = 0;
    account1.id    = "aaaaa";
    account1.name  = "WATA";

    account2.index = 1;
    account2.id    = "bbbbb";
    account2.name  = "Zorro";

    accounts[account1.hash()] = account1;
    accounts[account2.hash()] = account2;

    database->writeAccounts(accounts);
    accounts = database->readAccounts();

    // clang-format off
    ASSERT_EQ(accounts.size(),                                    2);
    ASSERT_EQ(accounts["594f803b380a41396ed63dca39503542"].index, 0);
    ASSERT_EQ(accounts["594f803b380a41396ed63dca39503542"].id,    "aaaaa");
    ASSERT_EQ(accounts["594f803b380a41396ed63dca39503542"].name,  "WATA");
    ASSERT_EQ(accounts["a21075a36eeddd084e17611a238c7101"].index, 1);
    ASSERT_EQ(accounts["a21075a36eeddd084e17611a238c7101"].id,    "bbbbb");
    ASSERT_EQ(accounts["a21075a36eeddd084e17611a238c7101"].name,  "Zorro");
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
