#include "src/widgets/logsfilterwidget/logsfilterwidget.h"
#include "ui_logsfilterwidget.h"

#include <gtest/gtest.h>



class Test_LogsFilterWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logsFilterWidget = new LogsFilterWidget();
    }

    void TearDown() override
    {
        delete logsFilterWidget;
    }

    LogsFilterWidget* logsFilterWidget;
};



TEST_F(Test_LogsFilterWidget, Test_constructor_and_destructor)
{
}
