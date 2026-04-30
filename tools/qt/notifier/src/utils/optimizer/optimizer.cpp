#include "src/utils/optimizer/optimizer.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



Optimizer::Optimizer() :
    IOptimizer()
{
    qDebug() << "Create Optimizer";
}

Optimizer::~Optimizer()
{
    qDebug() << "Destroy Optimizer";
}

struct OptimizeNotificationsInfo
{
    explicit OptimizeNotificationsInfo(const QList<NotificationInfo>& _notifications)
    {
        notificationsArray = _notifications.constData();
    }

    const NotificationInfo* notificationsArray;
};

static void optimizeNotificationsForParallel(
    QThread* parentThread, int /*threadId*/, NotificationInfo* res, int /*size*/, int start, int end, void* additionalArgs
)
{
    const OptimizeNotificationsInfo* optimizeNotificationsInfo = reinterpret_cast<OptimizeNotificationsInfo*>(additionalArgs);

    const NotificationInfo* notificationsArray = optimizeNotificationsInfo->notificationsArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        res[i] = notificationsArray[i];
    }
}

QList<NotificationInfo> Optimizer::optimizeNotifications(const QList<NotificationInfo>& notifications, int optimizeSize)
{
    QList<NotificationInfo> res;
    res.resizeForOverwrite(optimizeSize);

    OptimizeNotificationsInfo optimizeNotificationsInfo(notifications);
    processInParallel(QThread::currentThread(), res, optimizeNotificationsForParallel, &optimizeNotificationsInfo);

    return res;
}
