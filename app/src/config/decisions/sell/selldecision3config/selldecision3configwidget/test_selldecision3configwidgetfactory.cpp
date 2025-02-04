#include "src/config/decisions/sell/selldecision3configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/iselldecision3config_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



TEST(Test_SellDecision3ConfigWidgetFactory, Test_constructor_and_destructor)
{
    SellDecision3ConfigWidgetFactory factory;
}

TEST(Test_SellDecision3ConfigWidgetFactory, Test_newInstance)
{
    SellDecision3ConfigWidgetFactory factory;

    StrictMock<SellDecision3ConfigMock> sellDecision3ConfigMock;

    ISellDecision3ConfigWidget *widget = factory.newInstance(&sellDecision3ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
