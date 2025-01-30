#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision3configwidgetfactory.h"



TEST(Test_BuyDecision3ConfigWidgetFactory, Test_constructor_and_destructor)
{
    BuyDecision3ConfigWidgetFactory factory;
}

TEST(Test_BuyDecision3ConfigWidgetFactory, Test_newInstance)
{
    BuyDecision3ConfigWidgetFactory factory;

    IBuyDecision3ConfigWidget *widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
