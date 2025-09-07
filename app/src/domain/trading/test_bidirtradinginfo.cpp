#include "src/domain/trading/bidirtradinginfo.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
class Test_BiDirTradingInfo : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};



TEST_F(Test_BiDirTradingInfo, Test_constructor_and_destructor)
{
    const BiDirTradingInfo info;

    // clang-format off
    ASSERT_EQ(info.turnover, 0);
    ASSERT_EQ(info.cause,    "");
    // clang-format on

    const BiDirTradingInfo info2(1, "a");

    // clang-format off
    ASSERT_EQ(info2.turnover, 1);
    ASSERT_EQ(info2.cause,    "a");
    // clang-format on
}

TEST_F(Test_BiDirTradingInfo, Test_copy_constructor)
{
    BiDirTradingInfo info;

    info.turnover = 1;
    info.cause    = "a";

    const BiDirTradingInfo info2(info);

    // clang-format off
    ASSERT_EQ(info2.turnover, 1);
    ASSERT_EQ(info2.cause,    "a");
    // clang-format on
}

TEST_F(Test_BiDirTradingInfo, Test_assign)
{
    BiDirTradingInfo info;
    BiDirTradingInfo info2;

    info.turnover = 1;
    info.cause    = "a";

    info2 = info;

    // clang-format off
    ASSERT_EQ(info2.turnover, 1);
    ASSERT_EQ(info2.cause,    "a");
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
