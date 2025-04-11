#include "src/widgets/orderwaveswidget/orderwaveswidgetfactory.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_OrderWavesWidgetFactory, Test_constructor_and_destructor)
{
    const OrderWavesWidgetFactory factory;
}

TEST(Test_OrderWavesWidgetFactory, Test_newInstance)
{
    const OrderWavesWidgetFactory factory;

    const IOrderWavesWidget* widget = factory.newInstance(2, 0.01f, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
// NOLINTEND(readability-magic-numbers)
