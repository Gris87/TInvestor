#include "src/widgets/orderwaveswidget/orderwaveswidget.h"

#include <gtest/gtest.h>



class Test_OrderWavesWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        orderWavesWidget = new OrderWavesWidget(2, 0.01f);
    }

    void TearDown()
    {
        delete orderWavesWidget;
    }

    OrderWavesWidget* orderWavesWidget;
};



TEST_F(Test_OrderWavesWidget, Test_constructor_and_destructor)
{
}
