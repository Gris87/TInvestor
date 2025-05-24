#include "src/widgets/accountchartwidget/charttooltip.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_ChartTooltip : public ::testing::Test
{
protected:
    void SetUp() override
    {
        tooltip = new ChartTooltip(nullptr);
    }

    void TearDown() override
    {
        delete tooltip;
    }

    ChartTooltip* tooltip;
};



TEST_F(Test_ChartTooltip, Test_constructor_and_destructor)
{
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
