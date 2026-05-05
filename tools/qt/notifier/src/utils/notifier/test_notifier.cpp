#include "src/utils/notifier/notifier.h"

#include <gtest/gtest.h>

#include "src/widgets/notificationwidget/inotificationwidgetfactory_mock.h"



using ::testing::StrictMock;



class Test_Notifier : public ::testing::Test
{
protected:
    void SetUp() override
    {
        notificationWidgetFactoryMock = new StrictMock<NotificationWidgetFactoryMock>();

        notifier = new Notifier(notificationWidgetFactoryMock);
    }

    void TearDown() override
    {
        delete notifier;
        delete notificationWidgetFactoryMock;
    }

    Notifier*                                  notifier;
    StrictMock<NotificationWidgetFactoryMock>* notificationWidgetFactoryMock;
};



TEST_F(Test_Notifier, Test_constructor_and_destructor)
{
}
