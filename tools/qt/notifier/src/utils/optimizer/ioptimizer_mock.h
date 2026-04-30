#pragma once



#include "src/utils/optimizer/ioptimizer.h"

#include <gmock/gmock.h>



class OptimizerMock : public IOptimizer
{
public:
    OptimizerMock() :
        IOptimizer()
    {
    }
    ~OptimizerMock() override = default;

    OptimizerMock(const OptimizerMock& another)            = delete;
    OptimizerMock& operator=(const OptimizerMock& another) = delete;

    MOCK_METHOD(
        QList<NotificationInfo>,
        optimizeNotifications,
        (const QList<NotificationInfo>& notifications, int optimizeSize),
        (override)
    );
};
