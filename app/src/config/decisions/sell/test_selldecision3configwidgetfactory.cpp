#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision3configwidgetfactory.h"



TEST(Test_SellDecision3ConfigWidgetFactory, Test_constructor_and_destructor)
{
    SellDecision3ConfigWidgetFactory factory;
}

TEST(Test_SellDecision3ConfigWidgetFactory, Test_newInstance)
{
    SellDecision3ConfigWidgetFactory factory;

    ISellDecision3ConfigWidget *widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
