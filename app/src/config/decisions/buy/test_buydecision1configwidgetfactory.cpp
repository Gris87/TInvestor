#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1configwidgetfactory.h"



TEST(Test_BuyDecision1ConfigWidgetFactory, Test_constructor_and_destructor)
{
    BuyDecision1ConfigWidgetFactory factory;
}

TEST(Test_BuyDecision1ConfigWidgetFactory, Test_newInstance)
{
    BuyDecision1ConfigWidgetFactory factory;

    IBuyDecision1ConfigWidget *widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
