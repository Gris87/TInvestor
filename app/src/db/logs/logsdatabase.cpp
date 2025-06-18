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
    explicit FindLogsIndeciesInfo(const QByteArray& _content) :
        content(_content)
    {
        const int cpuCount = QThread::idealThreadCount();

        results.resize(cpuCount);
    }

    QByteArray        content;
    QList<QList<int>> results;
};

static void
findLogsIndeciesForParallel(QThread* parentThread, int threadId, QList<int>& /*temp*/, int start, int end, void* additionalArgs)
{
    FindLogsIndeciesInfo* findLogsIndeciesInfo = reinterpret_cast<FindLogsIndeciesInfo*>(additionalArgs);

    const char* contentArray = findLogsIndeciesInfo->content.constData();
    QList<int>* resultsArray = findLogsIndeciesInfo->results.data();

    const int part = findLogsIndeciesInfo->content.size() / findLogsIndeciesInfo->results.size();

    start = part * threadId;
    end   = qMin(part * (threadId + 1), findLogsIndeciesInfo->content.size() - 3);

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
    explicit MergeLogsIndeciesInfo(const QList<QList<int>>& _results) :
        results(_results)
    {
        indecies.resizeForOverwrite(results.size() + 1);

        int index = 0;

        for (int i = 0; i < results.size(); ++i)
        {
            indecies[i] = index;

            index += results.at(i).size();
        }

        indecies[results.size()] = index;
    }

    QList<int>        indecies;
    QList<QList<int>> results;
};

static void mergeLogsIndeciesForParallel(
    QThread* parentThread, int threadId, QList<int>& res, int /*start*/, int /*end*/, void* additionalArgs
)
{
    MergeLogsIndeciesInfo* mergeLogsIndeciesInfo = reinterpret_cast<MergeLogsIndeciesInfo*>(additionalArgs);

    const int         index   = mergeLogsIndeciesInfo->indecies.at(threadId);
    const QList<int>& results = mergeLogsIndeciesInfo->results.at(threadId);

    int* resArray = res.data();

    for (int i = 0; i < results.size() && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[index + i] = results.at(i);
    }
}

struct ReadLogsInfo
{
    explicit ReadLogsInfo(ILogosStorage* _logosStorage, const QByteArray& _content, QList<int>* _indecies) :
        logosStorage(_logosStorage),
        content(_content),
        indecies(_indecies)
    {
    }

    ILogosStorage* logosStorage;
    QByteArray     content;
    QList<int>*    indecies;
};

static void
readLogsForParallel(QThread* parentThread, int /*threadId*/, QList<LogEntry>& res, int start, int end, void* additionalArgs)
{
    ReadLogsInfo* readLogsInfo = reinterpret_cast<ReadLogsInfo*>(additionalArgs);

    ILogosStorage*   logosStorage  = readLogsInfo->logosStorage;
    const QByteArray content       = readLogsInfo->content;
    int*             indeciesArray = readLogsInfo->indecies->data();

    LogEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        LogEntry& entry = resArray[res.size() - i - 1];

        const int startBlock = i > 0 ? indeciesArray[i - 1] + 3 : 0;
        const int endBlock   = indeciesArray[i];

        const QByteArray entryContent = content.mid(startBlock, endBlock - startBlock + 1);
        simdjson::padded_string jsonData(entryContent.toStdString());

        simdjson::ondemand::parser parser;

        try
        {
            simdjson::ondemand::document doc = parser.iterate(jsonData);

            entry.fromJsonObject(doc.get_object());

            logosStorage->lock(); // TODO: Should do read lock outside
            entry.instrumentLogo = logosStorage->getLogo(entry.instrumentId);
            logosStorage->unlock();
        }
        catch (...)
        {
            qWarning() << "Failed to parse log entry";
        }
    }
}

QList<LogEntry> LogsDatabase::readLogs()
{
    qDebug() << "Reading logs from database";

    QList<LogEntry> res;

    const std::shared_ptr<IFile> logsFile = mFileFactory->newInstance(logsDirPath() + "/logs.json");

    if (logsFile->open(QIODevice::ReadOnly))
    {
        QByteArray content = logsFile->readAll();
        logsFile->close();

        if (content.size() > MIN_FILE_SIZE_PARALLEL)
        {
            QList<int> indecies;

            FindLogsIndeciesInfo findLogsIndeciesInfo(content);
            processInParallel(indecies, findLogsIndeciesForParallel, &findLogsIndeciesInfo);

            MergeLogsIndeciesInfo mergeLogsIndeciesInfo(findLogsIndeciesInfo.results);
            indecies.resizeForOverwrite(mergeLogsIndeciesInfo.indecies.constLast() + 1);
            processInParallel(indecies, mergeLogsIndeciesForParallel, &mergeLogsIndeciesInfo);

            indecies[indecies.size() - 1] = content.size() - 1;

            res.resizeForOverwrite(indecies.size());

            ReadLogsInfo readLogsInfo(mLogosStorage, content, &indecies);
            processInParallel(res, readLogsForParallel, &readLogsInfo);
        }
        else
        {
            content = "[" + content + "]";
            simdjson::padded_string jsonData(content.toStdString());

            simdjson::ondemand::parser parser;

            try
            {
                simdjson::ondemand::document doc = parser.iterate(jsonData);

                simdjson::ondemand::array jsonLogs = doc.get_array();
                res.resizeForOverwrite(jsonLogs.count_elements());

                int i = res.size() - 1;

                mLogosStorage->lock();

                for (simdjson::ondemand::object jsonObject : jsonLogs)
                {
                    LogEntry& entry = res[i];

                    entry.fromJsonObject(jsonObject);
                    entry.instrumentLogo = mLogosStorage->getLogo(entry.instrumentId);

                    --i;
                }

                mLogosStorage->unlock();
            }
            catch (...)
            {
                qWarning() << "Failed to parse logs";
            }
        }
    }

    return res;
}

void LogsDatabase::appendLog(const LogEntry& entry)
{
    const QString dirPath = logsDirPath();

    const std::shared_ptr<IDir> dir = mDirFactory->newInstance();

    bool ok = dir->mkpath(dirPath);
    Q_ASSERT_X(ok, "LogsDatabase::appendLog()", "Failed to create dir");

    const std::shared_ptr<IFile> logsFile = mFileFactory->newInstance(dirPath + "/logs.json");

    ok = logsFile->open(QIODevice::WriteOnly | QIODevice::Append);
    Q_ASSERT_X(ok, "LogsDatabase::appendLog()", "Failed to open file");

    const QJsonDocument jsonDoc(entry.toJsonObject());

    if (logsFile->size() > 0)
    {
        logsFile->write(",\n");
    }

    logsFile->write(jsonDoc.toJson(QJsonDocument::Compact));
    logsFile->close();
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
