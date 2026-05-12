#include "src/utils/notifier/notifier.h"

#include <gtest/gtest.h>

#include "src/widgets/notificationwidget/inotificationwidget_mock.h"
#include "src/widgets/notificationwidget/inotificationwidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::Return;
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

TEST_F(Test_Notifier, Test_setEnabled)
{
    notifier->setEnabled(true);
}

TEST_F(Test_Notifier, Test_setFilter)
{
    const Filter filter;

    notifier->setFilter(filter);
}

TEST_F(Test_Notifier, Test_notificationsAdded_and_resetNotificationWidgets)
{
    const InSequence seq;

    StrictMock<NotificationWidgetMock> notificationWidgetMock1;
    StrictMock<NotificationWidgetMock> notificationWidgetMock2;
    StrictMock<NotificationWidgetMock> notificationWidgetMock3;
    StrictMock<NotificationWidgetMock> notificationWidgetMock4;

    QList<NotificationInfo> notifications;
    NotificationInfo        notification1;
    NotificationInfo        notification2;
    NotificationInfo        notification3;
    NotificationInfo        notification4;

    notification1.requestTimestamp = 1704056400000;
    notification1.timestamp        = 1704056400000;
    notification1.messageType      = MESSAGE_TYPE_SYSTEM;
    notification1.text             = "Hello";

    notification2.requestTimestamp = 1704056460000;
    notification2.timestamp        = 1704056460000;
    notification2.messageType      = MESSAGE_TYPE_HUGE_SELL;
    notification2.text             = "Darkness";

    notification3.requestTimestamp = 1704056520000;
    notification3.timestamp        = 1704056520000;
    notification3.messageType      = MESSAGE_TYPE_DIVIDENDS;
    notification3.text             = "Smile";

    notification4.requestTimestamp = 1704056580000;
    notification4.timestamp        = 1704056580000;
    notification4.messageType      = MESSAGE_TYPE_PORTFOLIO;
    notification4.text             = "Face";

    notifications << notification1 << notification2 << notification3 << notification4;

    EXPECT_CALL(*notificationWidgetFactoryMock, newInstance(QString("Hello"), nullptr))
        .WillOnce(Return(&notificationWidgetMock1));
    EXPECT_CALL(*notificationWidgetFactoryMock, newInstance(QString("Darkness"), nullptr))
        .WillOnce(Return(&notificationWidgetMock2));
    EXPECT_CALL(*notificationWidgetFactoryMock, newInstance(QString("Smile"), nullptr))
        .WillOnce(Return(&notificationWidgetMock3));
    EXPECT_CALL(*notificationWidgetFactoryMock, newInstance(QString("..."), nullptr)).WillOnce(Return(&notificationWidgetMock4));

    notifier->notificationsAdded(notifications);

    notifier->resetNotificationWidgets();
}

TEST_F(Test_Notifier, Test_notificationWidgetDestroyed)
{
    notifier->notificationWidgetDestroyed(nullptr);
}
