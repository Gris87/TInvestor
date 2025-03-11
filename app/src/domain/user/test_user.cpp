#include "src/domain/user/user.h"

#include <gtest/gtest.h>



TEST(Test_User, Test_constructor_and_destructor)
{
    User user;

    // clang-format off
    ASSERT_EQ(user.token,                "");
    ASSERT_EQ(user.qualified,            false);
    ASSERT_EQ(user.qualifiedForWorkWith, QStringList());
    ASSERT_EQ(user.tariff,               "");
    ASSERT_NEAR(user.commission,         0.0f, 0.0001f);
    // clang-format on
}

TEST(Test_User, Test_copy_constructor)
{
    User user;

    user.token                = "TOKEN";
    user.qualified            = true;
    user.qualifiedForWorkWith = QStringList() << "hello" << "world";
    user.tariff               = "admin";
    user.commission           = 0.01f;

    User user2(user);

    // clang-format off
    ASSERT_EQ(user2.token,                "TOKEN");
    ASSERT_EQ(user2.qualified,            true);
    ASSERT_EQ(user2.qualifiedForWorkWith, QStringList() << "hello" << "world");
    ASSERT_EQ(user2.tariff,               "admin");
    ASSERT_NEAR(user2.commission,         0.01f, 0.0001f);
    // clang-format on
}

TEST(Test_User, Test_assign)
{
    User user;
    User user2;

    user.token                = "TOKEN";
    user.qualified            = true;
    user.qualifiedForWorkWith = QStringList() << "hello" << "world";
    user.tariff               = "admin";
    user.commission           = 0.01f;

    user2 = user;

    // clang-format off
    ASSERT_EQ(user2.token,                "TOKEN");
    ASSERT_EQ(user2.qualified,            true);
    ASSERT_EQ(user2.qualifiedForWorkWith, QStringList() << "hello" << "world");
    ASSERT_EQ(user2.tariff,               "admin");
    ASSERT_NEAR(user2.commission,         0.01f, 0.0001f);
    // clang-format on
}

TEST(Test_User, Test_setTariff)
{
    User user;

    ASSERT_EQ(user.tariff, "");
    ASSERT_NEAR(user.commission, 0.0f, 0.0001f);

    user.setTariff("fees");

    ASSERT_EQ(user.tariff, "fees");
    ASSERT_NEAR(user.commission, 0.3f, 0.0001f);

    user.setTariff("trader");

    ASSERT_EQ(user.tariff, "trader");
    ASSERT_NEAR(user.commission, 0.05f, 0.0001f);

    user.setTariff("premium");

    ASSERT_EQ(user.tariff, "premium");
    ASSERT_NEAR(user.commission, 0.04f, 0.0001f);

    user.setTariff("sandbox");

    ASSERT_EQ(user.tariff, "sandbox");
    ASSERT_NEAR(user.commission, 0.04f, 0.0001f);

    user.setTariff("blah");

    ASSERT_EQ(user.tariff, "blah");
    ASSERT_NEAR(user.commission, 0.00f, 0.0001f);
}
