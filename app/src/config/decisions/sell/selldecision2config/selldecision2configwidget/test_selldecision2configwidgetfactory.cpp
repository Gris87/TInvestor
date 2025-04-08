#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h"



using ::testing::StrictMock;



TEST(Test_SellDecision2ConfigWidgetFactory, Test_constructor_and_destructor)
{
    const SellDecision2ConfigWidgetFactory factory;
}

TEST(Test_SellDecision2ConfigWidgetFactory, Test_newInstance)
{
    const SellDecision2ConfigWidgetFactory factory;

    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    ISellDecision2ConfigWidget* widget = factory.newInstance(&sellDecision2ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
