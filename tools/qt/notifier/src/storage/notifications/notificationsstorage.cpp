#include "src/storage/notifications/notificationsstorage.h"

#include <QDebug>



NotificationsStorage::NotificationsStorage(INotificationsDatabase* notificationsDatabase) :
    INotificationsStorage(),
    mRwMutex(new QReadWriteLock()),
    mNotificationsDatabase(notificationsDatabase),
    mNotifications()
{
    qDebug() << "Create NotificationsStorage";
}

NotificationsStorage::~NotificationsStorage()
{
    qDebug() << "Destroy NotificationsStorage";

    delete mRwMutex;
}

void NotificationsStorage::readFromDatabase()
{
    mNotifications = mNotificationsDatabase->readNotifications();
}

void NotificationsStorage::readLock()
{
    mRwMutex->lockForRead();
}

void NotificationsStorage::readUnlock()
{
    mRwMutex->unlock();
}

void NotificationsStorage::writeLock()
{
    mRwMutex->lockForWrite();
}

void NotificationsStorage::writeUnlock()
{
    mRwMutex->unlock();
}

const QList<NotificationInfo>& NotificationsStorage::getNotifications()
{
    return mNotifications;
}
