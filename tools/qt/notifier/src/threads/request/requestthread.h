#pragma once



#include "src/threads/request/irequestthread.h"

#include "src/config/iconfig.h"
#include "src/storage/notifications/inotificationsstorage.h"
#include "src/utils/http/ihttpclient.h"



class RequestThread : public IRequestThread
{
    Q_OBJECT

public:
    explicit RequestThread(
        IConfig* config, INotificationsStorage* notificationsStorage, IHttpClient* httpClient, QObject* parent = nullptr
    );
    ~RequestThread() override;

    RequestThread(const RequestThread& another)            = delete;
    RequestThread& operator=(const RequestThread& another) = delete;

    void run() override;

    void terminateThread() override;

    void processNotificationsResponse(const QByteArray& resp);

private:
    IConfig*               mConfig;
    INotificationsStorage* mNotificationsStorage;
    IHttpClient*           mHttpClient;
};
