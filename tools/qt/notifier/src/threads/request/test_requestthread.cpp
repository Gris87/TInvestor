#include "src/threads/request/requestthread.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"
#include "src/db/notifications/inotificationsdatabase_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/optimizer/ioptimizer_mock.h"



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



MATCHER_P(IsNotificationsEqWithoutTimestamp, another, "")
{
    if (arg.size() != another.size())
    {
        return false;
    }

    for (int i = 0; i < arg.size(); ++i)
    {
        NotificationInfo        notification        = arg.at(i);
        const NotificationInfo& anotherNotification = another.at(i);

        notification.requestTimestamp = anotherNotification.requestTimestamp;

        if (notification != anotherNotification)
        {
            return false;
        }
    }

    return true;
}



class Test_RequestThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        configMock                = new StrictMock<ConfigMock>();
        notificationsDatabaseMock = new StrictMock<NotificationsDatabaseMock>();
        httpClientMock            = new StrictMock<HttpClientMock>();
        optimizerMock             = new StrictMock<OptimizerMock>();

        thread = new RequestThread(configMock, notificationsDatabaseMock, httpClientMock, optimizerMock);
    }

    void TearDown() override
    {
        delete thread;
        delete configMock;
        delete notificationsDatabaseMock;
        delete httpClientMock;
        delete optimizerMock;
    }

    RequestThread*                         thread;
    StrictMock<ConfigMock>*                configMock;
    StrictMock<NotificationsDatabaseMock>* notificationsDatabaseMock;
    StrictMock<HttpClientMock>*            httpClientMock;
    StrictMock<OptimizerMock>*             optimizerMock;
};



TEST_F(Test_RequestThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_RequestThread, Test_run)
{
    const InSequence seq;

    QList<NotificationInfo> notifications;

    HttpResult httpResult1;
    httpResult1.statusCode = 200;
    httpResult1.body =
        R"({"notifications":[{"timestamp":1704056460000,"type":"system","text":"Everything crashes","data":""},{"timestamp":1704056520000,"type":"dividends","text":"Hello my subscribers","data":""}]})";

    HttpResult httpResult2;
    httpResult2.statusCode = 200;
    httpResult2.body =
        R"({"notifications":[{"timestamp":1704056580000,"type":"portfolio","text":"You are rich","data":"Some log"}]})";

    HttpResult badHttpResult;
    badHttpResult.statusCode = 200;
    badHttpResult.body       = "{Bad content ::::: 555";

    QList<NotificationInfo> notificationsForWrite;
    NotificationInfo        notification1;
    NotificationInfo        notification2;

    notification1.requestTimestamp = 1704056520000;
    notification1.timestamp        = 1704056520000;
    notification1.messageType      = MESSAGE_TYPE_DIVIDENDS;
    notification1.text             = "Hello my subscribers";
    notification1.data             = "";

    notification2.requestTimestamp = 1704056460000;
    notification2.timestamp        = 1704056460000;
    notification2.messageType      = MESSAGE_TYPE_SYSTEM;
    notification2.text             = "Everything crashes";
    notification2.data             = "";

    notificationsForWrite << notification1 << notification2;

    QList<NotificationInfo> notificationsForAppend;
    NotificationInfo        notificationBeforeRemoveAttachment;
    NotificationInfo        notificationAfterRemoveAttachment;

    notificationBeforeRemoveAttachment.requestTimestamp = 0;
    notificationBeforeRemoveAttachment.timestamp        = 1704056580000;
    notificationBeforeRemoveAttachment.messageType      = MESSAGE_TYPE_PORTFOLIO;
    notificationBeforeRemoveAttachment.text             = "You are rich";
    notificationBeforeRemoveAttachment.data             = "Some log";

    notificationAfterRemoveAttachment      = notificationBeforeRemoveAttachment;
    notificationAfterRemoveAttachment.data = "+";

    notificationsForAppend << notificationAfterRemoveAttachment;

    EXPECT_CALL(*notificationsDatabaseMock, readNotifications()).WillOnce(Return(notifications));
    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("localhost"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(443));
    EXPECT_CALL(*httpClientMock, get(QUrl("https://localhost:443/notifications?from=10"), IHttpClient::Headers()))
        .WillOnce(Return(httpResult1));
    EXPECT_CALL(*notificationsDatabaseMock, writeNotifications(IsNotificationsEqWithoutTimestamp(notificationsForWrite)));

    thread->run();

    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("localhost"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(443));
    EXPECT_CALL(*httpClientMock, get(QUrl("https://localhost:443/notifications?from=1704056520010"), IHttpClient::Headers()))
        .WillOnce(Return(httpResult2));
    EXPECT_CALL(*notificationsDatabaseMock, writeAttachment(notificationBeforeRemoveAttachment));
    EXPECT_CALL(*notificationsDatabaseMock, appendNotifications(IsNotificationsEqWithoutTimestamp(notificationsForAppend)));

    thread->run();

    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("localhost"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(443));
    EXPECT_CALL(*httpClientMock, get(QUrl("https://localhost:443/notifications?from=1704056580010"), IHttpClient::Headers()))
        .WillOnce(Return(badHttpResult));

    thread->run();
}

TEST_F(Test_RequestThread, Test_terminateThread)
{
    thread->terminateThread();
}

TEST_F(Test_RequestThread, Test_optimize)
{
    const InSequence seq;

    QList<NotificationInfo> notifications;
    QList<NotificationInfo> optimizedNotifications;

    notifications.resizeForOverwrite(11);
    optimizedNotifications.resizeForOverwrite(5);

    thread->testSetLimitNotifications(notifications.size() - 1);
    thread->testSetOptimizeSize(optimizedNotifications.size());

    for (int i = 0; i < notifications.size(); ++i)
    {
        NotificationInfo& notification = notifications[i];

        notification.requestTimestamp = notifications.size() - i;
        notification.timestamp        = notifications.size() - i;
        notification.messageType      = MESSAGE_TYPE_SYSTEM;
        notification.text             = "Hello";
        notification.data             = "Bye";
    }

    for (int i = 0; i < optimizedNotifications.size(); ++i)
    {
        NotificationInfo& notification = optimizedNotifications[i];

        notification.requestTimestamp = notifications.size() - i;
        notification.timestamp        = notifications.size() - i;
        notification.messageType      = MESSAGE_TYPE_SYSTEM;
        notification.text             = "Hello";
        notification.data             = "Bye";
    }

    HttpResult httpResult;
    httpResult.statusCode = 200;
    httpResult.body       = "{}";

    EXPECT_CALL(*notificationsDatabaseMock, readNotifications()).WillOnce(Return(notifications));
    EXPECT_CALL(*configMock, getServerAddress()).WillOnce(Return("localhost"));
    EXPECT_CALL(*configMock, getServerPort()).WillOnce(Return(443));
    EXPECT_CALL(*httpClientMock, get(QUrl("https://localhost:443/notifications?from=21"), IHttpClient::Headers()))
        .WillOnce(Return(httpResult));
    EXPECT_CALL(*notificationsDatabaseMock, readNotifications()).WillOnce(Return(notifications));
    EXPECT_CALL(*optimizerMock, optimizeNotifications(notifications, 5)).WillOnce(Return(optimizedNotifications));
    EXPECT_CALL(*notificationsDatabaseMock, writeNotifications(optimizedNotifications));

    thread->run();
}
