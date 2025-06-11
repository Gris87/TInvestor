#include "src/widgets/logsfilterwidget/logsfilterwidgetfactory.h"

#include <gtest/gtest.h>



TEST(Test_LogsFilterWidgetFactory, Test_constructor_and_destructor)
{
    const LogsFilterWidgetFactory factory;
}

TEST(Test_LogsFilterWidgetFactory, Test_newInstance)
{
    const LogsFilterWidgetFactory factory;

    const ILogsFilterWidget* widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
