#include "src/threads/request/requestthread.h"

#include <QDebug>
#include <QUrlQuery>



constexpr int HTTP_STATUS_CODE_OK = 200;



RequestThread::RequestThread(
    IConfig* config, INotificationsStorage* notificationsStorage, IHttpClient* httpClient, QObject* parent
) :
    IRequestThread(parent),
    mConfig(config),
    mNotificationsStorage(notificationsStorage),
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

    const qint64 fromTimestamp = 0;

    QUrl url =
        QUrl(QString("http://%1:%2/notifications").arg(mConfig->getServerAddress(), QString::number(mConfig->getServerPort())));

    QUrlQuery query;
    query.addQueryItem("from", QString::number(fromTimestamp));

    url.setQuery(query.query());

    const IHttpClient::Headers headers;
    bool                       success = false;

    while (!QThread::currentThread()->isInterruptionRequested() && !success)
    {
        const HttpResult httpResult = mHttpClient->get(url, headers);

        if (httpResult.statusCode == HTTP_STATUS_CODE_OK)
        {
            processNotificationsResponse(httpResult.body);

            success = true;
        }
    }

    qDebug() << "Finish RequestThread";
}

void RequestThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void RequestThread::processNotificationsResponse(const QByteArray& resp)
{
    qInfo() << QString::fromUtf8(resp);
}
