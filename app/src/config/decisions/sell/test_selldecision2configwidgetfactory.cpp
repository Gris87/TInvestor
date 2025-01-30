#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision2configwidgetfactory.h"



TEST(Test_SellDecision2ConfigWidgetFactory, Test_constructor_and_destructor)
{
    SellDecision2ConfigWidgetFactory factory;
}

TEST(Test_SellDecision2ConfigWidgetFactory, Test_newInstance)
{
    SellDecision2ConfigWidgetFactory factory;

    ISellDecision2ConfigWidget *widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
