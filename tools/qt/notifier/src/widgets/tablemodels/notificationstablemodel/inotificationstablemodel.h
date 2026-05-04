#pragma once



#include <QAbstractTableModel>

#include "src/domain/filter/filter.h"
#include "src/domain/notification/notificationinfo.h"



enum NotificationsTableColumns : qint8
{
    NOTIFICATIONS_TIME_COLUMN,
    NOTIFICATIONS_TYPE_COLUMN,
    NOTIFICATIONS_TEXT_COLUMN,
    NOTIFICATIONS_COLUMN_COUNT,
};



class INotificationsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit INotificationsTableModel(QObject* parent = nullptr) :
        QAbstractTableModel(parent)
    {
    }
    ~INotificationsTableModel() override = default;

    INotificationsTableModel(const INotificationsTableModel& another)            = delete;
    INotificationsTableModel& operator=(const INotificationsTableModel& another) = delete;

    virtual void setFilter(const Filter& filter)                                  = 0;
    virtual void notificationsRead(const QList<NotificationInfo>& notifications)  = 0;
    virtual void notificationsAdded(const QList<NotificationInfo>& notifications) = 0;
    virtual void refreshBackground()                                              = 0;
};
