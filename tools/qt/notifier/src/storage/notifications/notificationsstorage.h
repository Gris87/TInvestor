#pragma once



#include "src/storage/notifications/inotificationsstorage.h"

#include <QReadWriteLock>

#include "src/db/notifications/inotificationsdatabase.h"



class NotificationsStorage : public INotificationsStorage
{
public:
    explicit NotificationsStorage(INotificationsDatabase* notificationsDatabase);
    ~NotificationsStorage() override;

    NotificationsStorage(const NotificationsStorage& another)            = delete;
    NotificationsStorage& operator=(const NotificationsStorage& another) = delete;

    void                           readFromDatabase() override;
    void                           readLock() override;
    void                           readUnlock() override;
    void                           writeLock() override;
    void                           writeUnlock() override;
    const QList<NotificationInfo>& getNotifications() override;

private:
    QReadWriteLock*         mRwMutex;
    INotificationsDatabase* mNotificationsDatabase;
    QList<NotificationInfo> mNotifications;
};
