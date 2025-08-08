#include "src/db/logs/logsdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"
#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr int MIN_FILE_SIZE_PARALLEL = 5000;



LogsDatabase::LogsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory, ILogosStorage* logosStorage, bool autoPilotMode) :
    ILogsDatabase(),
    mDirFactory(dirFactory),
    mFileFactory(fileFactory),
    mLogosStorage(logosStorage),
    mAutoPilotMode(autoPilotMode),
    mAccountHash()
{
    qDebug() << "Create LogsDatabase";
}

LogsDatabase::~LogsDatabase()
{
    qDebug() << "Destroy LogsDatabase";
}

void LogsDatabase::setAccount(const QString& account)
{
    mAccountHash = account;
}

struct FindLogsIndeciesInfo
{
    explicit FindLogsIndeciesInfo(const QByteArray& _content)
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

static void findLogsIndeciesForParallel(
    QThread* parentThread, int threadId, int* /*temp*/, int /*size*/, int start, int end, void* additionalArgs
)
{
    FindLogsIndeciesInfo* findLogsIndeciesInfo = reinterpret_cast<FindLogsIndeciesInfo*>(additionalArgs);

    const char*  contentArray = findLogsIndeciesInfo->contentArray;
    const qint64 contentSize  = findLogsIndeciesInfo->contentSize;
    QList<int>*  resultsArray = findLogsIndeciesInfo->resultsArray;
    const int    resultsSize  = findLogsIndeciesInfo->results.size();

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

struct MergeLogsIndeciesInfo
{
    explicit MergeLogsIndeciesInfo(const QList<QList<int>>& _results)
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

static void mergeLogsIndeciesForParallel(
    QThread* parentThread, int threadId, int* res, int /*size*/, int /*start*/, int /*end*/, void* additionalArgs
)
{
    MergeLogsIndeciesInfo* mergeLogsIndeciesInfo = reinterpret_cast<MergeLogsIndeciesInfo*>(additionalArgs);

    const int         index   = mergeLogsIndeciesInfo->indeciesArray[threadId];
    const QList<int>& results = mergeLogsIndeciesInfo->resultsArray[threadId];

    const int* resultsArray = results.constData();
    const int  resultsSize  = results.size();

    for (int i = 0; i < resultsSize && !parentThread->isInterruptionRequested(); ++i)
    {
        res[index + i] = resultsArray[i];
    }
}

struct ReadLogsInfo
{
    explicit ReadLogsInfo(ILogosStorage* _logosStorage, const QByteArray& _content, const QList<int>& _indecies) :
        logosStorage(_logosStorage),
        content(_content)
    {
        indeciesArray = _indecies.constData();
    }

    ILogosStorage*    logosStorage;
    const QByteArray& content;
    const int*        indeciesArray;
};

static void
readLogsForParallel(QThread* parentThread, int /*threadId*/, LogEntry* res, int size, int start, int end, void* additionalArgs)
{
    ReadLogsInfo* readLogsInfo = reinterpret_cast<ReadLogsInfo*>(additionalArgs);

    ILogosStorage*    logosStorage  = readLogsInfo->logosStorage;
    const QByteArray& content       = readLogsInfo->content;
    const int*        indeciesArray = readLogsInfo->indeciesArray;

    simdjson::ondemand::parser parser;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        LogEntry& entry = res[size - i - 1];

        const int startBlock = i > 0 ? indeciesArray[i - 1] + 3 : 0;
        const int endBlock   = indeciesArray[i];

        const QByteArray              entryContent = content.mid(startBlock, endBlock - startBlock + 1);
        const simdjson::padded_string jsonData(entryContent.toStdString());

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            entry.fromJsonObject(doc.get_object());
            entry.instrumentLogo = logosStorage->getLogo(entry.instrumentId);
        }
        catch (...)
        {
            qWarning() << "Failed to parse log entry";
        }
    }
}

QList<LogEntry> LogsDatabase::readLogs(int partId)
{
    qDebug() << "Reading logs from database";

    QList<LogEntry> res;

    const std::shared_ptr<IFile> logsFile = mFileFactory->newInstance(QString("%1/%2").arg(logsDirPath(), fileName(partId)));

    if (logsFile->open(QIODevice::ReadOnly))
    {
        QByteArray content = logsFile->readAll();
        logsFile->close();

        if (content.size() > MIN_FILE_SIZE_PARALLEL)
        {
            QList<int> indecies;

            FindLogsIndeciesInfo findLogsIndeciesInfo(content);
            processInParallel(QThread::currentThread(), indecies, findLogsIndeciesForParallel, &findLogsIndeciesInfo);

            MergeLogsIndeciesInfo mergeLogsIndeciesInfo(findLogsIndeciesInfo.results);
            indecies.resizeForOverwrite(mergeLogsIndeciesInfo.indecies.constLast() + 1);
            processInParallel(QThread::currentThread(), indecies, mergeLogsIndeciesForParallel, &mergeLogsIndeciesInfo);

            indecies[indecies.size() - 1] = content.size() - 1;

            res.resizeForOverwrite(indecies.size());

            mLogosStorage->readLock();

            ReadLogsInfo readLogsInfo(mLogosStorage, content, indecies);
            processInParallel(QThread::currentThread(), res, readLogsForParallel, &readLogsInfo);

            mLogosStorage->readUnlock();
        }
        else
        {
            content = "[" + content + "]";
            const simdjson::padded_string jsonData(content.toStdString());

            simdjson::ondemand::parser parser;

            try
            {
                simdjson::ondemand::document doc = parser.iterate(jsonData);

                simdjson::ondemand::array jsonLogs = doc.get_array();
                res.resizeForOverwrite(jsonLogs.count_elements());

                int i = res.size() - 1;

                mLogosStorage->readLock();

                for (const simdjson::ondemand::object jsonObject : jsonLogs)
                {
                    LogEntry& entry = res[i];

                    entry.fromJsonObject(jsonObject);
                    entry.instrumentLogo = mLogosStorage->getLogo(entry.instrumentId);

                    --i;
                }

                mLogosStorage->readUnlock();
            }
            catch (...)
            {
                qWarning() << "Failed to parse logs";
            }
        }
    }

    return res;
}

struct WriteLogsInfo
{
    explicit WriteLogsInfo()
    {
        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    QList<QByteArray> results;
    QByteArray*       resultsArray;
};

static void writeLogsForParallel(
    QThread* parentThread, int threadId, LogEntry* entries, int /*size*/, int start, int end, void* additionalArgs
)
{
    WriteLogsInfo* writeLogsInfo = reinterpret_cast<WriteLogsInfo*>(additionalArgs);

    QByteArray* resultsArray = writeLogsInfo->resultsArray;

    for (int i = end - 1; i >= start && !parentThread->isInterruptionRequested(); --i)
    {
        const QJsonDocument jsonDoc(entries[i].toJsonObject());

        resultsArray[threadId].append(jsonDoc.toJson(QJsonDocument::Compact));

        if (i > 0)
        {
            resultsArray[threadId].append(",\n");
        }
    }
}

void LogsDatabase::writeLogs(QList<LogEntry>& entries, int partId)
{
    qDebug() << "Writing logs to database";

    const QString dirPath = logsDirPath();

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(dirPath);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    const std::shared_ptr<IFile> logsFile = mFileFactory->newInstance(QString("%1/%2").arg(dirPath, fileName(partId)));

    ok = logsFile->open(QIODevice::WriteOnly);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    WriteLogsInfo writeLogsInfo;
    processInParallel(QThread::currentThread(), entries, writeLogsForParallel, &writeLogsInfo);

    for (int i = writeLogsInfo.results.size() - 1; i >= 0; --i)
    {
        logsFile->write(writeLogsInfo.resultsArray[i]);
    }

    logsFile->close();
}

void LogsDatabase::appendLog(const LogEntry& entry, int partId)
{
    const QString dirPath = logsDirPath();

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(dirPath);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to create dir");

    const std::shared_ptr<IFile> logsFile = mFileFactory->newInstance(QString("%1/%2").arg(dirPath, fileName(partId)));

    ok = logsFile->open(QIODevice::WriteOnly | QIODevice::Append);
    Q_ASSERT_X(ok, __FUNCTION__, "Failed to open file");

    const QJsonDocument jsonDoc(entry.toJsonObject());

    if (logsFile->size() > 0)
    {
        logsFile->write(",\n");
    }

    logsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    logsFile->close();
}

void LogsDatabase::deleteLogs(int partId)
{
    qDebug() << "Deleting logs";

    const std::shared_ptr<IFile> logsFile = mFileFactory->newInstance(QString("%1/%2").arg(logsDirPath(), fileName(partId)));

    logsFile->remove();
}

void LogsDatabase::setAutoPilotMode(bool autoPilotMode)
{
    mAutoPilotMode = autoPilotMode;
}

QString LogsDatabase::logsDirPath() const
{
    QString res;

    if (mAutoPilotMode)
    {
        res = QString("%1/data/autopilot/%2").arg(qApp->applicationDirPath(), mAccountHash);
    }
    else
    {
        res = QString("%1/data/simulator").arg(qApp->applicationDirPath());
    }

    return res;
}

QString LogsDatabase::fileName(int partId) const
{
    return partId < 0 ? "logs.json" : QString("logs%1.json").arg(partId);
}
