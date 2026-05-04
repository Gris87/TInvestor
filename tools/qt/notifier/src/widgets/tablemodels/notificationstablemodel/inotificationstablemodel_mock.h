#pragma once



#include "src/widgets/tablemodels/notificationstablemodel/inotificationstablemodel.h"

#include <gmock/gmock.h>



class NotificationsTableModelMock : public INotificationsTableModel
{
    Q_OBJECT

public:
    explicit NotificationsTableModelMock(QObject* parent = nullptr) :
        INotificationsTableModel(parent)
    {
    }
    ~NotificationsTableModelMock() override = default;

    NotificationsTableModelMock(const NotificationsTableModelMock& another)            = delete;
    NotificationsTableModelMock& operator=(const NotificationsTableModelMock& another) = delete;

    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(int, columnCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(QVariant, data, (const QModelIndex& index, int role), (const, override));

    MOCK_METHOD(void, setFilter, (const Filter& filter), (override));
    MOCK_METHOD(void, notificationsRead, (const QList<NotificationInfo>& notifications), (override));
    MOCK_METHOD(void, notificationsAdded, (const QList<NotificationInfo>& notifications), (override));
    MOCK_METHOD(void, refreshBackground, (), (override));
};
