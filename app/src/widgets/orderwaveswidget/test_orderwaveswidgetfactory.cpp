#include "src/widgets/orderwaveswidget/orderwaveswidgetfactory.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
class Test_OrderWavesWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new OrderWavesWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    OrderWavesWidgetFactory* factory;
};



TEST_F(Test_OrderWavesWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_OrderWavesWidgetFactory, Test_newInstance)
{
    const IOrderWavesWidget* widget = factory->newInstance(2, 0.01f, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
// NOLINTEND(readability-magic-numbers)
