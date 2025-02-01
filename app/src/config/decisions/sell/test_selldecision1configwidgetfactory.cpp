#include "src/config/decisions/sell/selldecision1configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/iselldecision1config_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



TEST(Test_SellDecision1ConfigWidgetFactory, Test_constructor_and_destructor)
{
    SellDecision1ConfigWidgetFactory factory;
}

TEST(Test_SellDecision1ConfigWidgetFactory, Test_newInstance)
{
    SellDecision1ConfigWidgetFactory factory;

    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;

    ISellDecision1ConfigWidget *widget = factory.newInstance(&sellDecision1ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
