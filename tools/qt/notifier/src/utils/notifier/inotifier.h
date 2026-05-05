#pragma once



#include "src/domain/filter/filter.h"
#include "src/domain/notification/notificationinfo.h"



class INotifier
{
public:
    INotifier()          = default;
    virtual ~INotifier() = default;

    INotifier(const INotifier& another)            = delete;
    INotifier& operator=(const INotifier& another) = delete;

    virtual void setEnabled(bool enabled)                                         = 0;
    virtual void setFilter(const Filter& filter)                                  = 0;
    virtual void notificationsAdded(const QList<NotificationInfo>& notifications) = 0;
};
