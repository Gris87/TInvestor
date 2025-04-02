#include "src/widgets/accountchartwidget/accountchartwidgetfactory.h"

#include <gtest/gtest.h>



TEST(Test_AccountChartWidgetFactory, Test_constructor_and_destructor)
{
    AccountChartWidgetFactory factory;
}

TEST(Test_AccountChartWidgetFactory, Test_newInstance)
{
    AccountChartWidgetFactory factory;

    IAccountChartWidget* widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
