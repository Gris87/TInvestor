#pragma once



#include "src/threads/request/irequestthread.h"

#include "src/config/iconfig.h"



class RequestThread : public IRequestThread
{
    Q_OBJECT

public:
    explicit RequestThread(IConfig* config, QObject* parent = nullptr);
    ~RequestThread() override;

    RequestThread(const RequestThread& another)            = delete;
    RequestThread& operator=(const RequestThread& another) = delete;

    void run() override;

    void terminateThread() override;

private:
    IConfig* mConfig;
};
