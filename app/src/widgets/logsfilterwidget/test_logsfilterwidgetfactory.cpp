#include "src/widgets/logsfilterwidget/logsfilterwidgetfactory.h"

#include <gtest/gtest.h>



class Test_LogsFilterWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new LogsFilterWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    LogsFilterWidgetFactory* factory;
};



TEST_F(Test_LogsFilterWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsFilterWidgetFactory, Test_newInstance)
{
    const ILogsFilterWidget* widget = factory->newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
