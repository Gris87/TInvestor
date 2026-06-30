#include "src/db/notifications/notificationsdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr int MIN_FILE_SIZE_PARALLEL = 5000;



NotificationsDatabase::NotificationsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory) :
    INotificationsDatabase(),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory)
{
    qDebug() << "Create NotificationsDatabase";
}

NotificationsDatabase::~NotificationsDatabase()
{
    qDebug() << "Destroy NotificationsDatabase";
}

struct FindNotificationsIndeciesInfo
{
    explicit FindNotificationsIndeciesInfo(const QByteArray& _content)
    {
        contentArray = _content.constData();
        contentSize  = _content.size();

        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    const char*       contentArray;
    qint64            contentSize;
    QList<QList<int>> results;
    QList<int>*       resultsArray;
};

static void findNotificationsIndeciesForParallel(
    QThread* parentThread, int threadId, int* /*temp*/, int /*size*/, int start, int end, void* additionalArgs
)
{
    const FindNotificationsIndeciesInfo* findNotificationsIndeciesInfo =
        reinterpret_cast<FindNotificationsIndeciesInfo*>(additionalArgs);

    const char*  contentArray = findNotificationsIndeciesInfo->contentArray;
    const qint64 contentSize  = findNotificationsIndeciesInfo->contentSize;
    QList<int>*  resultsArray = findNotificationsIndeciesInfo->resultsArray;
    const int    resultsSize  = findNotificationsIndeciesInfo->results.size();

    const int part = contentSize / resultsSize;

    start = part * threadId;
    end   = qMin(part * (threadId + 1), contentSize - 3);

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        if (contentArray[i] == '}' && contentArray[i + 1] == ',' &&
            (contentArray[i + 2] == '\n' || (contentArray[i + 2] == '\r' && contentArray[i + 3] == '\n')))
        {
            resultsArray[threadId].append(i);

            i += 3;
        }
    }
}

struct MergeNotificationsIndeciesInfo
{
    explicit MergeNotificationsIndeciesInfo(const QList<QList<int>>& _results)
    {
        indecies.resizeForOverwrite(_results.size() + 1);
        indeciesArray = indecies.data();
        resultsArray  = _results.constData();

        int index = 0;

        for (int i = 0; i < _results.size(); ++i)
        {
            indeciesArray[i] = index;

            index += resultsArray[i].size();
        }

        indeciesArray[_results.size()] = index;
    }

    QList<int>        indecies;
    int*              indeciesArray;
    const QList<int>* resultsArray;
};

static void mergeNotificationsIndeciesForParallel(
    QThread* parentThread, int threadId, int* res, int /*size*/, int /*start*/, int /*end*/, void* additionalArgs
)
{
    const MergeNotificationsIndeciesInfo* mergeNotificationsIndeciesInfo =
        reinterpret_cast<MergeNotificationsIndeciesInfo*>(additionalArgs);

    const int         index   = mergeNotificationsIndeciesInfo->indeciesArray[threadId];
    const QList<int>& results = mergeNotificationsIndeciesInfo->resultsArray[threadId];

    const int* resultsArray = results.constData();
    const int  resultsSize  = results.size();

    for (int i = 0; i < resultsSize && !parentThread->isInterruptionRequested(); ++i)
    {
        res[index + i] = resultsArray[i];
    }
}

struct ReadNotificationsInfo
{
    explicit ReadNotificationsInfo(const QByteArray& _content, const QList<int>& _indecies) :
        content(_content)
    {
        indeciesArray = _indecies.constData();
    }

    QByteArray content;
    const int* indeciesArray;
};

static void readNotificationsForParallel(
    QThread* parentThread, int /*threadId*/, NotificationInfo* res, int size, int start, int end, void* additionalArgs
)
{
    const ReadNotificationsInfo* readNotificationsInfo = reinterpret_cast<ReadNotificationsInfo*>(additionalArgs);

    const QByteArray& content       = readNotificationsInfo->content;
    const int*        indeciesArray = readNotificationsInfo->indeciesArray;

    simdjson::ondemand::parser parser;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        NotificationInfo& notification = res[size - i - 1];

        const int startBlock = i > 0 ? indeciesArray[i - 1] + 3 : 0;
        const int endBlock   = indeciesArray[i];

        const QByteArray              notificationContent = content.mid(startBlock, endBlock - startBlock + 1);
        const simdjson::padded_string jsonData(notificationContent.toStdString());

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            notification.fromJsonObject(doc.get_object());
        }
        catch (...)
        {
            qWarning() << "Failed to parse notification";
        }
    }
}

QList<NotificationInfo> NotificationsDatabase::readNotifications()
{
    qDebug() << "Reading notifications from database";

    QList<NotificationInfo> res;

    const std::shared_ptr<IFile> notificationsFile =
        mFileFactory->newInstance(QString("%1/data/notifications/notifications.json").arg(qApp->applicationDirPath()));

    if (notificationsFile->open(QIODevice::ReadOnly))
    {
        QByteArray content = notificationsFile->readAll();
        notificationsFile->close();

        if (content.size() > MIN_FILE_SIZE_PARALLEL)
        {
            QList<int> indecies;

            FindNotificationsIndeciesInfo findNotificationsIndeciesInfo(content);
            processInParallel(
                QThread::currentThread(), indecies, findNotificationsIndeciesForParallel, &findNotificationsIndeciesInfo
            );

            MergeNotificationsIndeciesInfo mergeNotificationsIndeciesInfo(findNotificationsIndeciesInfo.results);
            indecies.resizeForOverwrite(mergeNotificationsIndeciesInfo.indecies.constLast() + 1);
            processInParallel(
                QThread::currentThread(), indecies, mergeNotificationsIndeciesForParallel, &mergeNotificationsIndeciesInfo
            );

            indecies[indecies.size() - 1] = content.size() - 1;

            res.resizeForOverwrite(indecies.size());

            ReadNotificationsInfo readNotificationsInfo(content, indecies);
            processInParallel(QThread::currentThread(), res, readNotificationsForParallel, &readNotificationsInfo);
        }
        else
        {
            content = "[" + content + "]";
            const simdjson::padded_string jsonData(content.toStdString());

            simdjson::ondemand::parser parser;

            try
            {
                simdjson::ondemand::document doc = parser.iterate(jsonData);

                simdjson::ondemand::array jsonNotifications = doc.get_array();
                res.resizeForOverwrite(jsonNotifications.count_elements());

                int i = res.size() - 1;

                for (const simdjson::ondemand::object jsonObject : jsonNotifications)
                {
                    NotificationInfo& notification = res[i];

                    notification.fromJsonObject(jsonObject);

                    --i;
                }
            }
            catch (...)
            {
                qWarning() << "Failed to parse notifications";
            }
        }
    }

    return res;
}

struct WriteNotificationsInfo
{
    explicit WriteNotificationsInfo()
    {
        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    QList<QByteArray> results;
    QByteArray*       resultsArray;
};

static void writeNotificationsForParallel(
    QThread* parentThread, int threadId, NotificationInfo* notifications, int /*size*/, int start, int end, void* additionalArgs
)
{
    const WriteNotificationsInfo* writeNotificationsInfo = reinterpret_cast<WriteNotificationsInfo*>(additionalArgs);

    QByteArray* resultsArray = writeNotificationsInfo->resultsArray;

    for (int i = end - 1; i >= start && !parentThread->isInterruptionRequested(); --i)
    {
        const QJsonDocument jsonDoc(notifications[i].toJsonObject());

        resultsArray[threadId].append(jsonDoc.toJson(QJsonDocument::Compact));

        if (i > 0)
        {
            resultsArray[threadId].append(",\n");
        }
    }
}

void NotificationsDatabase::writeNotifications(QList<NotificationInfo>& notifications)
{
    qDebug() << "Writing notifications to database";

    const QString dirPath = QString("%1/data/notifications").arg(qApp->applicationDirPath());

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(dirPath);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    const std::shared_ptr<IFile> notificationsFile = mFileFactory->newInstance(QString("%1/notifications.json").arg(dirPath));

    ok = notificationsFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    WriteNotificationsInfo writeNotificationsInfo;
    processInParallel(QThread::currentThread(), notifications, writeNotificationsForParallel, &writeNotificationsInfo);

    for (int i = writeNotificationsInfo.results.size() - 1; i >= 0; --i)
    {
        notificationsFile->write(writeNotificationsInfo.resultsArray[i]);
    }

    notificationsFile->close();
}

void NotificationsDatabase::appendNotifications(const QList<NotificationInfo>& notifications)
{
    qDebug() << "Appending notifications to database";

    const std::shared_ptr<IFile> notificationsFile =
        mFileFactory->newInstance(QString("%1/data/notifications/notifications.json").arg(qApp->applicationDirPath()));

    const bool ok = notificationsFile->open(QIODevice::Append);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    for (int i = 0; i < notifications.size(); ++i)
    {
        const QJsonDocument jsonDoc(notifications.at(notifications.size() - i - 1).toJsonObject());

        notificationsFile->write(",\n");
        notificationsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    }

    notificationsFile->close();
}

void NotificationsDatabase::writeAttachment(const NotificationInfo& notification)
{
    qDebug() << "Writing attachment to database";

    const QString dirPath = QString("%1/data/attachments").arg(qApp->applicationDirPath());

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(dirPath);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    const std::shared_ptr<IFile> attachmentFile =
        mFileFactory->newInstance(QString("%1/%2.txt").arg(dirPath, QString::number(notification.timestamp)));

    ok = attachmentFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    attachmentFile->write(notification.data.toUtf8());
    attachmentFile->close();
}
