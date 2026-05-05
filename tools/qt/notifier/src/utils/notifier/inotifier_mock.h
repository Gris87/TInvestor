#pragma once



#include "src/utils/notifier/inotifier.h"

#include <gmock/gmock.h>



class NotifierMock : public INotifier
{
public:
    NotifierMock() :
        INotifier()
    {
    }
    ~NotifierMock() override = default;

    NotifierMock(const NotifierMock& another)            = delete;
    NotifierMock& operator=(const NotifierMock& another) = delete;

    MOCK_METHOD(void, setEnabled, (bool enabled), (override));
    MOCK_METHOD(void, setFilter, (const Filter& filter), (override));
    MOCK_METHOD(void, notificationsAdded, (const QList<NotificationInfo>& notifications), (override));
};
