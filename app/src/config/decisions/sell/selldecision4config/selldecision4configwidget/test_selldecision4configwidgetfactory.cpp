#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/selldecision4configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"



using ::testing::StrictMock;



TEST(Test_SellDecision4ConfigWidgetFactory, Test_constructor_and_destructor)
{
    const SellDecision4ConfigWidgetFactory factory;
}

TEST(Test_SellDecision4ConfigWidgetFactory, Test_newInstance)
{
    const SellDecision4ConfigWidgetFactory factory;

    StrictMock<SellDecision4ConfigMock> sellDecision4ConfigMock;

    ISellDecision4ConfigWidget* widget = factory.newInstance(&sellDecision4ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
