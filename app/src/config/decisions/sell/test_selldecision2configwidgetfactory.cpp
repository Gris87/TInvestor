#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision2configwidgetfactory.h"
#include "src/config/decisions/sell/iselldecision2config_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



TEST(Test_SellDecision2ConfigWidgetFactory, Test_constructor_and_destructor)
{
    SellDecision2ConfigWidgetFactory factory;
}

TEST(Test_SellDecision2ConfigWidgetFactory, Test_newInstance)
{
    SellDecision2ConfigWidgetFactory factory;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    ISellDecision2ConfigWidget *widget = factory.newInstance(&sellDecision2ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
