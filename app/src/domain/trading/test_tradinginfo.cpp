#include "src/domain/trading/tradinginfo.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
TEST(Test_TradingInfo, Test_constructor_and_destructor)
{
    const TradingInfo info;

    // clang-format off
    ASSERT_NEAR(info.expectedCost, 0, 0.0001);
    ASSERT_EQ(info.cause,          "");
    // clang-format on

    const TradingInfo info2(1.0, "a");

    // clang-format off
    ASSERT_NEAR(info2.expectedCost, 1, 0.0001);
    ASSERT_EQ(info2.cause,          "a");
    // clang-format on
}

TEST(Test_TradingInfo, Test_copy_constructor)
{
    TradingInfo info;

    info.expectedCost = 1.0;
    info.cause        = "a";

    const TradingInfo info2(info);

    // clang-format off
    ASSERT_NEAR(info2.expectedCost, 1.0, 0.0001);
    ASSERT_EQ(info2.cause,          "a");
    // clang-format on
}

TEST(Test_TradingInfo, Test_assign)
{
    TradingInfo info;
    TradingInfo info2;

    info.expectedCost = 1.0;
    info.cause        = "a";

    info2 = info;

    // clang-format off
    ASSERT_NEAR(info2.expectedCost, 1.0, 0.0001);
    ASSERT_EQ(info2.cause,          "a");
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
