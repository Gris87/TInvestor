#include "src/threads/request/requestthread.h"

#include <QDebug>
#include <QUrlQuery>



constexpr int LIMIT_NOTIFICATIONS = 1000000;
constexpr int OPTIMIZE_SIZE       = 100000;
constexpr int HTTP_STATUS_CODE_OK = 200;



RequestThread::RequestThread(
    IConfig*                config,
    INotificationsDatabase* notificationsDatabase,
    IHttpClient*            httpClient,
    IOptimizer*             optimizer,
    QObject*                parent
) :
    IRequestThread(parent),
    mConfig(config),
    mNotificationsDatabase(notificationsDatabase),
    mHttpClient(httpClient),
    mOptimizer(optimizer),
    mLastNotificationTimestamp(),
    mAmountOfEntries(),
    mLimitNotifications(LIMIT_NOTIFICATIONS),
    mOptimizeSize(OPTIMIZE_SIZE)
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

    readNotificationsAtFirstRun();
    requestNotifications();

    qDebug() << "Finish RequestThread";
}

void RequestThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();
}

void RequestThread::readNotificationsAtFirstRun()
{
    if (mAmountOfEntries == 0)
    {
        QList<NotificationInfo> notifications = mNotificationsDatabase->readNotifications();
        mAmountOfEntries                      = notifications.size();

        if (mAmountOfEntries > 0)
        {
            const NotificationInfo& lastNotification = notifications.constFirst(); // Since it reversed

            mLastNotificationTimestamp = lastNotification.timestamp;
        }

        emit notificationsRead(notifications);
    }
}

void RequestThread::requestNotifications()
{
    QUrl url =
        QUrl(QString("https://%1:%2/notifications").arg(mConfig->getServerAddress(), QString::number(mConfig->getServerPort())));

    QUrlQuery query;
    query.addQueryItem("from", QString::number(mLastNotificationTimestamp + 1));

    url.setQuery(query.query());

    const IHttpClient::Headers headers;
    const HttpResult           httpResult = mHttpClient->get(url, headers);

    if (httpResult.statusCode == HTTP_STATUS_CODE_OK)
    {
        optimize();
        processNotificationsResponse(httpResult.body);
    }
}

void RequestThread::processNotificationsResponse(const QByteArray& resp)
{
    const simdjson::padded_string jsonData(resp.toStdString());

    simdjson::ondemand::parser parser;

    try
    {
        simdjson::ondemand::document doc = parser.iterate(jsonData);

        simdjson::ondemand::object jsonObject        = doc.get_object();
        simdjson::ondemand::array  jsonNotifications = jsonObject["notifications"].get_array();

        QList<NotificationInfo> notifications;
        notifications.resizeForOverwrite(jsonNotifications.count_elements());

        if (!notifications.isEmpty())
        {
            int i = notifications.size() - 1;

            for (const simdjson::ondemand::object jsonObject : jsonNotifications)
            {
                NotificationInfo& notification = notifications[i];

                notification.fromJsonObject(jsonObject);

                --i;
            }

            if (mLastNotificationTimestamp == 0)
            {
                emit notificationsRead(notifications);
                mNotificationsDatabase->writeNotifications(notifications);
            }
            else
            {
                emit notificationsAdded(notifications);
                mNotificationsDatabase->appendNotifications(notifications);
            }

            mLastNotificationTimestamp  = notifications.constFirst().timestamp; // Since it reversed
            mAmountOfEntries           += notifications.size();
        }
    }
    catch (...)
    {
        qWarning() << "Failed to parse notifications";
    }
}

void RequestThread::optimize()
{
    if (mAmountOfEntries > mLimitNotifications)
    {
        QList<NotificationInfo> newNotifications =
            mOptimizer->optimizeNotifications(mNotificationsDatabase->readNotifications(), mOptimizeSize);
        mAmountOfEntries = newNotifications.size();

        emit notificationsRead(newNotifications);
        mNotificationsDatabase->writeNotifications(newNotifications);
    }
}
