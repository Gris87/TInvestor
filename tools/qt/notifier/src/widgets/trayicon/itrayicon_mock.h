#pragma once



#include "src/widgets/trayicon/itrayicon.h"

#include <gmock/gmock.h>



class TrayIconMock : public ITrayIcon
{
    Q_OBJECT

public:
    explicit TrayIconMock(QObject* parent = nullptr) :
        ITrayIcon(parent)
    {
    }
    ~TrayIconMock() override = default;

    TrayIconMock(const TrayIconMock& another)            = delete;
    TrayIconMock& operator=(const TrayIconMock& another) = delete;

    MOCK_METHOD(void, setFilter, (const Filter& filter), (override));
    MOCK_METHOD(void, resetCounter, (), (override));
    MOCK_METHOD(void, notificationsAdded, (const QList<NotificationInfo>& notifications), (override));
};
