#pragma once



#include "src/threads/request/irequestthread.h"

#include "src/config/iconfig.h"
#include "src/db/notifications/inotificationsdatabase.h"
#include "src/utils/http/ihttpclient.h"



class RequestThread : public IRequestThread
{
    Q_OBJECT

public:
    explicit RequestThread(
        IConfig* config, INotificationsDatabase* notificationsDatabase, IHttpClient* httpClient, QObject* parent = nullptr
    );
    ~RequestThread() override;

    RequestThread(const RequestThread& another)            = delete;
    RequestThread& operator=(const RequestThread& another) = delete;

    void run() override;

    void terminateThread() override;

    void readNotificationsAtFirstRun();
    void requestNotifications();
    void processNotificationsResponse(const QByteArray& resp);

private:
    IConfig*                mConfig;
    INotificationsDatabase* mNotificationsDatabase;
    IHttpClient*            mHttpClient;
    qint64                  mLastNotificationTimestamp;
    int                     mAmountOfEntries;
};
