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
    Stock stock;

    const BiDirTradingInfo info;

    // clang-format off
    ASSERT_EQ(info.stock, nullptr);
    ASSERT_EQ(info.mode,  BIDIR_MODE_NONE);
    ASSERT_EQ(info.cause, "");
    // clang-format on

    const BiDirTradingInfo info2(&stock, BIDIR_MODE_HUGE_SPREAD, "a");

    // clang-format off
    ASSERT_EQ(info2.stock, &stock);
    ASSERT_EQ(info2.mode,  BIDIR_MODE_HUGE_SPREAD);
    ASSERT_EQ(info2.cause, "a");
    // clang-format on
}

TEST_F(Test_BiDirTradingInfo, Test_copy_constructor)
{
    Stock stock;

    BiDirTradingInfo info;

    info.stock = &stock;
    info.mode  = BIDIR_MODE_HUGE_SPREAD;
    info.cause = "a";

    const BiDirTradingInfo info2(info);

    // clang-format off
    ASSERT_EQ(info2.stock, &stock);
    ASSERT_EQ(info2.mode,  BIDIR_MODE_HUGE_SPREAD);
    ASSERT_EQ(info2.cause, "a");
    // clang-format on
}

TEST_F(Test_BiDirTradingInfo, Test_assign)
{
    Stock stock;

    BiDirTradingInfo info;
    BiDirTradingInfo info2;

    info.stock = &stock;
    info.mode  = BIDIR_MODE_HUGE_SPREAD;
    info.cause = "a";

    info2 = info;

    // clang-format off
    ASSERT_EQ(info2.stock, &stock);
    ASSERT_EQ(info2.mode,  BIDIR_MODE_HUGE_SPREAD);
    ASSERT_EQ(info2.cause, "a");
    // clang-format on
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
