#include "src/domain/user/account.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-function-cognitive-complexity)
TEST(Test_Account, Test_constructor_and_destructor)
{
    const Account account;

    // clang-format off
    ASSERT_EQ(account.index, 0);
    ASSERT_EQ(account.id,    "");
    ASSERT_EQ(account.name,  "");
    // clang-format on
}

TEST(Test_Account, Test_copy_constructor)
{
    Account account;

    account.index = 1;
    account.id    = "a";
    account.name  = "b";

    const Account account2(account);

    // clang-format off
    ASSERT_EQ(account2.index, 1);
    ASSERT_EQ(account2.id,    "a");
    ASSERT_EQ(account2.name,  "b");
    // clang-format on
}

TEST(Test_Account, Test_assign)
{
    Account account;
    Account account2;

    account.index = 1;
    account.id    = "a";
    account.name  = "b";

    account2 = account;

    // clang-format off
    ASSERT_EQ(account2.index, 1);
    ASSERT_EQ(account2.id,    "a");
    ASSERT_EQ(account2.name,  "b");
    // clang-format on
}

TEST(Test_Account, Test_hash)
{
    Account account;

    account.id = "aaaaa";

    ASSERT_EQ(account.hash(), "594f803b380a41396ed63dca39503542");

    account.id = "bbbbb";

    ASSERT_EQ(account.hash(), "a21075a36eeddd084e17611a238c7101");
}

TEST(Test_Account, Test_equals)
{
    Account account;
    Account account2;

    account.index = 1;
    account.id    = "a";
    account.name  = "b";

    account2.index = 1;
    account2.id    = "a";
    account2.name  = "b";

    ASSERT_EQ(account, account2);

    account2.index = 1111;
    ASSERT_NE(account, account2);
    account2.index = 1;
    ASSERT_EQ(account, account2);

    account2.id = "aaaa";
    ASSERT_NE(account, account2);
    account2.id = "a";
    ASSERT_EQ(account, account2);

    account2.name = "bbbb";
    ASSERT_NE(account, account2);
    account2.name = "b";
    ASSERT_EQ(account, account2);
}
// NOLINTEND(readability-function-cognitive-complexity)
