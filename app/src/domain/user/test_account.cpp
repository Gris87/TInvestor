#include "src/domain/user/account.h"

#include <gtest/gtest.h>



TEST(Test_Account, Test_constructor_and_destructor)
{
    Account account;

    // clang-format off
    ASSERT_EQ(account.id,     "");
    ASSERT_EQ(account.idHash, "");
    ASSERT_EQ(account.name,   "");
    // clang-format on
}

TEST(Test_Account, Test_copy_constructor)
{
    Account account;

    account.id     = "a";
    account.idHash = "b";
    account.name   = "c";

    Account account2(account);

    // clang-format off
    ASSERT_EQ(account2.id,     "a");
    ASSERT_EQ(account2.idHash, "b");
    ASSERT_EQ(account2.name,   "c");
    // clang-format on
}

TEST(Test_Account, Test_assign)
{
    Account account;
    Account account2;

    account.id     = "a";
    account.idHash = "b";
    account.name   = "c";

    account2 = account;

    // clang-format off
    ASSERT_EQ(account2.id,     "a");
    ASSERT_EQ(account2.idHash, "b");
    ASSERT_EQ(account2.name,   "c");
    // clang-format on
}

TEST(Test_Account, Test_setId)
{
    Account account;

    ASSERT_EQ(account.id, "");
    ASSERT_EQ(account.idHash, "");

    account.setId("aaaaa");

    ASSERT_EQ(account.id, "aaaaa");
    ASSERT_EQ(account.idHash, "594f803b380a41396ed63dca39503542");

    account.setId("bbbbb");

    ASSERT_EQ(account.id, "bbbbb");
    ASSERT_EQ(account.idHash, "a21075a36eeddd084e17611a238c7101");
}

TEST(Test_Account, Test_equals)
{
    Account account;
    Account account2;

    account.id     = "a";
    account.idHash = "b";
    account.name   = "c";

    account2.id     = "a";
    account2.idHash = "b";
    account2.name   = "c";

    ASSERT_EQ(account, account2);

    account2.id = "aaaa";
    ASSERT_NE(account, account2);
    account2.id = "a";
    ASSERT_EQ(account, account2);

    account2.idHash = "bbbb";
    ASSERT_EQ(account, account2);
    account2.idHash = "b";
    ASSERT_EQ(account, account2);

    account2.name = "cccc";
    ASSERT_NE(account, account2);
    account2.name = "c";
    ASSERT_EQ(account, account2);
}
