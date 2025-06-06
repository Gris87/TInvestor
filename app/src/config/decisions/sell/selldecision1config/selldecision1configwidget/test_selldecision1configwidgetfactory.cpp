#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h"



using ::testing::StrictMock;



TEST(Test_SellDecision1ConfigWidgetFactory, Test_constructor_and_destructor)
{
    const SellDecision1ConfigWidgetFactory factory;
}

TEST(Test_SellDecision1ConfigWidgetFactory, Test_newInstance)
{
    const SellDecision1ConfigWidgetFactory factory;

    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;

    ISellDecision1ConfigWidget* widget = factory.newInstance(&sellDecision1ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
