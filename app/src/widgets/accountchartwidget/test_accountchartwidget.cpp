#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <gtest/gtest.h>



class Test_AccountChartWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        accountChartWidget = new AccountChartWidget();
    }

    void TearDown()
    {
        delete accountChartWidget;
    }

    AccountChartWidget* accountChartWidget;
};



TEST_F(Test_AccountChartWidget, Test_constructor_and_destructor)
{
}
