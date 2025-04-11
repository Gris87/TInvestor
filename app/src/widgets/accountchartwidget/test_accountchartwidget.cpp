#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AccountChartWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        accountChartWidget = new AccountChartWidget();
    }

    void TearDown() override
    {
        delete accountChartWidget;
    }

    AccountChartWidget* accountChartWidget;
};



TEST_F(Test_AccountChartWidget, Test_constructor_and_destructor)
{
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
