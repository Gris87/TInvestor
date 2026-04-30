#pragma once



#include "src/domain/notification/notificationinfo.h"



class IOptimizer
{
public:
    IOptimizer()          = default;
    virtual ~IOptimizer() = default;

    IOptimizer(const IOptimizer& another)            = delete;
    IOptimizer& operator=(const IOptimizer& another) = delete;

    virtual QList<NotificationInfo> optimizeNotifications(const QList<NotificationInfo>& notifications, int optimizeSize) = 0;
};
