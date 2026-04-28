#pragma once



#include "src/threads/request/irequestthread.h"

#include "src/config/iconfig.h"
#include "src/utils/http/ihttpclient.h"



class RequestThread : public IRequestThread
{
    Q_OBJECT

public:
    explicit RequestThread(IConfig* config, IHttpClient* httpClient, QObject* parent = nullptr);
    ~RequestThread() override;

    RequestThread(const RequestThread& another)            = delete;
    RequestThread& operator=(const RequestThread& another) = delete;

    void run() override;

    void terminateThread() override;

    void processNotificationsResponse(const QByteArray& resp);

private:
    IConfig*     mConfig;
    IHttpClient* mHttpClient;
};
