#pragma once



#include "src/db/notifications/inotificationsdatabase.h"

#include <gmock/gmock.h>



class NotificationsDatabaseMock : public INotificationsDatabase
{
public:
    explicit NotificationsDatabaseMock() :
        INotificationsDatabase()
    {
    }
    ~NotificationsDatabaseMock() override = default;

    NotificationsDatabaseMock(const NotificationsDatabaseMock& another)            = delete;
    NotificationsDatabaseMock& operator=(const NotificationsDatabaseMock& another) = delete;

    MOCK_METHOD(QList<NotificationInfo>, readNotifications, (), (override));
    MOCK_METHOD(void, writeNotifications, (QList<NotificationInfo> & notifications), (override));
    MOCK_METHOD(void, appendNotifications, (const QList<NotificationInfo>& notifications), (override));
    MOCK_METHOD(void, writeAttachment, (const NotificationInfo& notification), (override));
};
