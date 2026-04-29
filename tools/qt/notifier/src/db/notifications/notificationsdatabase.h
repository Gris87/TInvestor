#pragma once



#include "src/db/notifications/inotificationsdatabase.h"

#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class NotificationsDatabase : public INotificationsDatabase
{
public:
    explicit NotificationsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory);
    ~NotificationsDatabase() override;

    NotificationsDatabase(const NotificationsDatabase& another)            = delete;
    NotificationsDatabase& operator=(const NotificationsDatabase& another) = delete;

    QList<NotificationInfo> readNotifications() override;
    void                    writeNotifications(QList<NotificationInfo>& notifications) override;
    void                    appendNotifications(const QList<NotificationInfo>& notifications) override;

private:
    IDirFactory*  mDirFactory;
    IFileFactory* mFileFactory;
};
