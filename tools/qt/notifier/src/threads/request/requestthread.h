#pragma once



#include "src/threads/request/irequestthread.h"

#include "src/config/iconfig.h"
#include "src/db/notifications/inotificationsdatabase.h"
#include "src/utils/http/ihttpclient.h"
#include "src/utils/optimizer/ioptimizer.h"



class RequestThread : public IRequestThread
{
    Q_OBJECT

public:
    explicit RequestThread(
        IConfig*                config,
        INotificationsDatabase* notificationsDatabase,
        IHttpClient*            httpClient,
        IOptimizer*             optimizer,
        QObject*                parent = nullptr
    );
    ~RequestThread() override;

    RequestThread(const RequestThread& another)            = delete;
    RequestThread& operator=(const RequestThread& another) = delete;

    void run() override;

    void terminateThread() override;

    void readNotificationsAtFirstRun();
    void requestNotifications();
    void processNotificationsResponse(const QByteArray& resp);

#ifdef TESTING_MODE
    void testSetLimitNotifications(int limitNotifications)
    {
        mLimitNotifications = limitNotifications;
    }

    void testSetOptimizeSize(int optimizeSize)
    {
        mOptimizeSize = optimizeSize;
    }
#endif

private:
    void optimize();

    IConfig*                mConfig;
    INotificationsDatabase* mNotificationsDatabase;
    IHttpClient*            mHttpClient;
    IOptimizer*             mOptimizer;
    qint64                  mLastNotificationTimestamp;
    int                     mAmountOfEntries;
    int                     mLimitNotifications;
    int                     mOptimizeSize;
};
