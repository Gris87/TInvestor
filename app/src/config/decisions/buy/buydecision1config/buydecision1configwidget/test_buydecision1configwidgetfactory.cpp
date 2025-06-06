#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h"



using ::testing::StrictMock;



TEST(Test_BuyDecision1ConfigWidgetFactory, Test_constructor_and_destructor)
{
    const BuyDecision1ConfigWidgetFactory factory;
}

TEST(Test_BuyDecision1ConfigWidgetFactory, Test_newInstance)
{
    const BuyDecision1ConfigWidgetFactory factory;

    StrictMock<BuyDecision1ConfigMock> buyDecision1ConfigMock;

    IBuyDecision1ConfigWidget* widget = factory.newInstance(&buyDecision1ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
