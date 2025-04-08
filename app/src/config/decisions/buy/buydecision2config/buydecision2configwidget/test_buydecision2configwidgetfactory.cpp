#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h"



using ::testing::StrictMock;



TEST(Test_BuyDecision2ConfigWidgetFactory, Test_constructor_and_destructor)
{
    const BuyDecision2ConfigWidgetFactory factory;
}

TEST(Test_BuyDecision2ConfigWidgetFactory, Test_newInstance)
{
    const BuyDecision2ConfigWidgetFactory factory;

    StrictMock<BuyDecision2ConfigMock> buyDecision2ConfigMock;

    IBuyDecision2ConfigWidget* widget = factory.newInstance(&buyDecision2ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
