#include "src/widgets/notificationwidget/notificationwidget.h"
#include "ui_notificationwidget.h"

#include <gtest/gtest.h>



class Test_NotificationWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        notificationWidget = new NotificationWidget();
    }

    void TearDown() override
    {
        delete notificationWidget;
    }

    NotificationWidget* notificationWidget;
};



TEST_F(Test_NotificationWidget, Test_constructor_and_destructor)
{
}
