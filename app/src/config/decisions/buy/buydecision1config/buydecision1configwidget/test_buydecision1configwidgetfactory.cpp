#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



TEST(Test_BuyDecision1ConfigWidgetFactory, Test_constructor_and_destructor)
{
    BuyDecision1ConfigWidgetFactory factory;
}

TEST(Test_BuyDecision1ConfigWidgetFactory, Test_newInstance)
{
    BuyDecision1ConfigWidgetFactory factory;

    StrictMock<BuyDecision1ConfigMock> buyDecision1ConfigMock;

    IBuyDecision1ConfigWidget *widget = factory.newInstance(&buyDecision1ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
