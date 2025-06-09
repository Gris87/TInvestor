#include "src/threads/logs/logsthread.h"

#include <QDebug>



LogsThread::LogsThread(ILogsDatabase* logsDatabase, QObject* parent) :
    ILogsThread(parent),
    mSemaphore(),
    mMutex(new QMutex()),
    mLogsDatabase(logsDatabase),
    mAccountId(),
    mEntries()
{
    qDebug() << "Create LogsThread";
}

LogsThread::~LogsThread()
{
    qDebug() << "Destroy LogsThread";

    delete mMutex;
}

void LogsThread::run()
{
    qDebug() << "Running LogsThread";

    blockSignals(false);
    readLogs();

    while (true)
    {
        mSemaphore.acquire();

        const LogEntry entry = takeEntry();

        if (entry.timestamp == 0)
        {
            break;
        }

        emit logAdded(entry);

        mLogsDatabase->appendLog(entry);
    }

    qDebug() << "Finish LogsThread";
}

void LogsThread::setAccountId(const QString& account, const QString& accountId)
{
    mLogsDatabase->setAccount(account);

    mAccountId = accountId;
}

void LogsThread::addLog(LogLevel level, const QString& instrumentId, const QString& message)
{
    if (isRunning())
    {
        LogEntry entry;

        entry.timestamp    = QDateTime::currentMSecsSinceEpoch();
        entry.level        = level;
        entry.instrumentId = instrumentId;
        entry.message      = message;

        const QMutexLocker lock(mMutex);
        mEntries.append(entry);

        mSemaphore.release();
    }
}

void LogsThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();

    const QMutexLocker lock(mMutex);
    mEntries.append(LogEntry());

    mSemaphore.release();
}

void LogsThread::readLogs()
{
    const QList<LogEntry> entries = mLogsDatabase->readLogs();

    emit logsRead(entries);
}

LogEntry LogsThread::takeEntry()
{
    const QMutexLocker lock(mMutex);

    return mEntries.takeFirst();
}
