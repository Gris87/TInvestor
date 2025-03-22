#include "src/widgets/orderwaveswidget/orderwaveswidgetfactory.h"

#include <gtest/gtest.h>



TEST(Test_OrderWavesWidgetFactory, Test_constructor_and_destructor)
{
    OrderWavesWidgetFactory factory;
}

TEST(Test_OrderWavesWidgetFactory, Test_newInstance)
{
    OrderWavesWidgetFactory factory;

    IOrderWavesWidget* widget = factory.newInstance(0.01f, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
