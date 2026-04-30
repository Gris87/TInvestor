#pragma once



#include "src/utils/optimizer/ioptimizer.h"



class Optimizer : public IOptimizer
{
public:
    Optimizer();
    ~Optimizer() override;

    Optimizer(const Optimizer& another)            = delete;
    Optimizer& operator=(const Optimizer& another) = delete;

    QList<NotificationInfo> optimizeNotifications(const QList<NotificationInfo>& notifications, int optimizeSize) override;
};
