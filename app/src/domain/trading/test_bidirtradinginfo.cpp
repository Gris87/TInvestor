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

    ASSERT_EQ(info.cause, "");

    const BiDirTradingInfo info2("a");

    ASSERT_EQ(info2.cause, "a");
}

TEST_F(Test_BiDirTradingInfo, Test_copy_constructor)
{
    BiDirTradingInfo info;

    info.cause = "a";

    const BiDirTradingInfo info2(info);

    ASSERT_EQ(info2.cause, "a");
}

TEST_F(Test_BiDirTradingInfo, Test_assign)
{
    BiDirTradingInfo info;
    BiDirTradingInfo info2;

    info.cause = "a";

    info2 = info;

    ASSERT_EQ(info2.cause, "a");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-magic-numbers)
