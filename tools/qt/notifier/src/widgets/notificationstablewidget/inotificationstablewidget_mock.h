#pragma once



#include "src/widgets/notificationstablewidget/inotificationstablewidget.h"

#include <gmock/gmock.h>



class NotificationsTableWidgetMock : public INotificationsTableWidget
{
    Q_OBJECT

public:
    explicit NotificationsTableWidgetMock(QWidget* parent = nullptr) :
        INotificationsTableWidget(parent)
    {
    }
    ~NotificationsTableWidgetMock() override = default;

    NotificationsTableWidgetMock(const NotificationsTableWidgetMock& another)            = delete;
    NotificationsTableWidgetMock& operator=(const NotificationsTableWidgetMock& another) = delete;

    MOCK_METHOD(void, setFilter, (const Filter& filter), (override));
    MOCK_METHOD(void, notificationsRead, (const QList<NotificationInfo>& notifications), (override));
    MOCK_METHOD(void, notificationsAdded, (const QList<NotificationInfo>& notifications), (override));
    MOCK_METHOD(void, refreshBackground, (), (override));

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
