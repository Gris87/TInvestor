#pragma once



#include <QThread>

#include "src/domain/notification/notificationinfo.h"



class IRequestThread : public QThread
{
    Q_OBJECT

public:
    explicit IRequestThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IRequestThread() override = default;

    IRequestThread(const IRequestThread& another)            = delete;
    IRequestThread& operator=(const IRequestThread& another) = delete;

    virtual void terminateThread() = 0;

signals:
    void requestCompleted();
    void notificationsRead(const QList<NotificationInfo>& notifications);
    void notificationsAdded(const QList<NotificationInfo>& notifications);
};
