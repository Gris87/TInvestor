#pragma once



#include <QObject>

#include "src/domain/filter/filter.h"
#include "src/domain/notification/notificationinfo.h"



class INotifier : public QObject
{
    Q_OBJECT

public:
    explicit INotifier(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    ~INotifier() override = default;

    INotifier(const INotifier& another)            = delete;
    INotifier& operator=(const INotifier& another) = delete;

    virtual void setEnabled(bool enabled)                                         = 0;
    virtual void setFilter(const Filter& filter)                                  = 0;
    virtual void resetNotificationWidgets()                                       = 0;
    virtual void notificationsAdded(const QList<NotificationInfo>& notifications) = 0;

signals:
    void notificationClicked();
};
