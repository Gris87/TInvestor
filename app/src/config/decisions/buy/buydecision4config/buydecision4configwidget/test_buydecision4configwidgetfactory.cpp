#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/buydecision4configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision4config/ibuydecision4config_mock.h"



using ::testing::StrictMock;



TEST(Test_BuyDecision4ConfigWidgetFactory, Test_constructor_and_destructor)
{
    const BuyDecision4ConfigWidgetFactory factory;
}

TEST(Test_BuyDecision4ConfigWidgetFactory, Test_newInstance)
{
    const BuyDecision4ConfigWidgetFactory factory;

    StrictMock<BuyDecision4ConfigMock> buyDecision4ConfigMock;

    IBuyDecision4ConfigWidget* widget = factory.newInstance(&buyDecision4ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
