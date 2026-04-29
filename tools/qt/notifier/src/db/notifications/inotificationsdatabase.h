#pragma once



#include "src/domain/notification/notificationinfo.h"



class INotificationsDatabase
{
public:
    explicit INotificationsDatabase() = default;
    virtual ~INotificationsDatabase() = default;

    INotificationsDatabase(const INotificationsDatabase& another)            = delete;
    INotificationsDatabase& operator=(const INotificationsDatabase& another) = delete;

    virtual QList<NotificationInfo> readNotifications()                                               = 0;
    virtual void                    writeNotifications(QList<NotificationInfo>& notifications)        = 0;
    virtual void                    appendNotifications(const QList<NotificationInfo>& notifications) = 0;
};
