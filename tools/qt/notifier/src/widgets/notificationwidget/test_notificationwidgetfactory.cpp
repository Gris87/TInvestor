#include "src/widgets/notificationwidget/notificationwidgetfactory.h"

#include <gtest/gtest.h>



class Test_NotificationWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new NotificationWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    NotificationWidgetFactory* factory;
};



TEST_F(Test_NotificationWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_NotificationWidgetFactory, Test_newInstance)
{
    const INotificationWidget* widget = factory->newInstance("", nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
