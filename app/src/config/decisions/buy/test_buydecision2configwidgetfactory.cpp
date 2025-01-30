#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision2configwidgetfactory.h"



TEST(Test_BuyDecision2ConfigWidgetFactory, Test_constructor_and_destructor)
{
    BuyDecision2ConfigWidgetFactory factory;
}

TEST(Test_BuyDecision2ConfigWidgetFactory, Test_newInstance)
{
    BuyDecision2ConfigWidgetFactory factory;

    IBuyDecision2ConfigWidget *widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
