#include "src/threads/logs/logsthread.h"

#include <QDebug>



LogsThread::LogsThread(IUserStorage* userStorage, ILogsDatabase* logsDatabase, QObject* parent) :
    ILogsThread(parent),
    mSemaphore(),
    mMutex(new QMutex()),
    mUserStorage(userStorage),
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

    if (mAccountId != "")
    {
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
    }
    else
    {
        emit accountNotFound();
    }

    qDebug() << "Finish LogsThread";
}

void LogsThread::setAccount(const QString& account)
{
    mLogsDatabase->setAccount(account);

    const QMutexLocker lock(mUserStorage->getMutex());
    const Accounts     accounts = mUserStorage->getAccounts();

    mAccountId = accounts.value(account).id;
}

void LogsThread::addLog(LogLevel level, const QString& message)
{
    LogEntry entry;

    entry.timestamp = QDateTime::currentMSecsSinceEpoch();
    entry.level     = level;
    entry.message   = message;

    const QMutexLocker lock(mMutex);
    mEntries.append(entry);

    mSemaphore.release();
}

void LogsThread::terminateThread()
{
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
