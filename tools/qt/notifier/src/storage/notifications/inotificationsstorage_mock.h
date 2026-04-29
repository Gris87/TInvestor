#pragma once



#include "src/storage/notifications/inotificationsstorage.h"

#include <gmock/gmock.h>



class NotificationsStorageMock : public INotificationsStorage
{
public:
    NotificationsStorageMock() :
        INotificationsStorage()
    {
    }
    ~NotificationsStorageMock() override = default;

    NotificationsStorageMock(const NotificationsStorageMock& another)            = delete;
    NotificationsStorageMock& operator=(const NotificationsStorageMock& another) = delete;

    MOCK_METHOD(void, readFromDatabase, (), (override));
    MOCK_METHOD(void, readLock, (), (override));
    MOCK_METHOD(void, readUnlock, (), (override));
    MOCK_METHOD(void, writeLock, (), (override));
    MOCK_METHOD(void, writeUnlock, (), (override));
    MOCK_METHOD(const QList<NotificationInfo>&, getNotifications, (), (override));
};
