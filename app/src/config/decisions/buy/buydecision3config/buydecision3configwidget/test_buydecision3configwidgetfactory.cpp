#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_BuyDecision3ConfigWidgetFactory, Test_constructor_and_destructor)
{
    BuyDecision3ConfigWidgetFactory factory;
}

TEST(Test_BuyDecision3ConfigWidgetFactory, Test_newInstance)
{
    BuyDecision3ConfigWidgetFactory factory;

    StrictMock<BuyDecision3ConfigMock> buyDecision3ConfigMock;

    IBuyDecision3ConfigWidget* widget = factory.newInstance(&buyDecision3ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
