#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision1configwidgetfactory.h"



TEST(Test_SellDecision1ConfigWidgetFactory, Test_constructor_and_destructor)
{
    SellDecision1ConfigWidgetFactory factory;
}

TEST(Test_SellDecision1ConfigWidgetFactory, Test_newInstance)
{
    SellDecision1ConfigWidgetFactory factory;

    ISellDecision1ConfigWidget *widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
