#include "src/widgets/tableitems/loglevel/logleveltableitemwidget.h"
#include "ui_logleveltableitemwidget.h"

#include <gtest/gtest.h>



class Test_LogLevelTableItemWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        widget = new LogLevelTableItemWidget();
    }

    void TearDown() override
    {
        delete widget;
    }

    LogLevelTableItemWidget* widget;
};



TEST_F(Test_LogLevelTableItemWidget, Test_constructor_and_destructor)
{
}
