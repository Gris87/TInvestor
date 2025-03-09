#include "src/widgets/filterwidget/filterwidgetfactory.h"

#include <gtest/gtest.h>



TEST(Test_FilterWidgetFactory, Test_constructor_and_destructor)
{
    FilterWidgetFactory factory;
}

TEST(Test_FilterWidgetFactory, Test_newInstance)
{
    FilterWidgetFactory factory;

    IFilterWidget* widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
