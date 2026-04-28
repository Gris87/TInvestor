#include "src/threads/request/requestthread.h"

#include <QDateTime>
#include <QDebug>



RequestThread::RequestThread(IConfig* config, IHttpClient* httpClient, QObject* parent) :
    IRequestThread(parent),
    mConfig(config),
    mHttpClient(httpClient)
{
    qDebug() << "Create RequestThread";
}

RequestThread::~RequestThread()
{
    qDebug() << "Destroy RequestThread";
}

void RequestThread::run()
{
    qDebug() << "Running RequestThread";

    blockSignals(false);

    qDebug() << "Finish RequestThread";
}

void RequestThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}
