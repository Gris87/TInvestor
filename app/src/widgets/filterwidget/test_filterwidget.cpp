#include "src/widgets/filterwidget/filterwidget.h"

#include <gtest/gtest.h>



class Test_FilterWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        filterWidget = new FilterWidget();
    }

    void TearDown()
    {
        delete filterWidget;
    }

    FilterWidget* filterWidget;
};



TEST_F(Test_FilterWidget, Test_constructor_and_destructor)
{
}
