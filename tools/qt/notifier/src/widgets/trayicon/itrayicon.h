#pragma once



#include <QSystemTrayIcon>

#include "src/domain/filter/filter.h"
#include "src/domain/notification/notificationinfo.h"



class ITrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit ITrayIcon(QObject* parent = nullptr) :
        QSystemTrayIcon(parent)
    {
    }
    ~ITrayIcon() override = default;

    ITrayIcon(const ITrayIcon& another)            = delete;
    ITrayIcon& operator=(const ITrayIcon& another) = delete;

    virtual void setFilter(const Filter& filter)                                  = 0;
    virtual void resetCounter()                                                   = 0;
    virtual void notificationsAdded(const QList<NotificationInfo>& notifications) = 0;

signals:
    void trayIconShowClicked();
    void trayIconExitClicked();
};
