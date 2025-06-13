#include "src/threads/logs/logsthread.h"

#include <QDebug>



LogsThread::LogsThread(
    ILogsDatabase* logsDatabase, IInstrumentsStorage* instrumentsStorage, ILogosStorage* logosStorage, QObject* parent
) :
    ILogsThread(parent),
    mSemaphore(),
    mMutex(new QMutex()),
    mLogsDatabase(logsDatabase),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mAccountId(),
    mLastLogTimestamp(),
    mAmountOfLogsWithSameTimestamp(),
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
        const qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

        if (timestamp == mLastLogTimestamp)
        {
            ++mAmountOfLogsWithSameTimestamp;
        }
        else
        {
            mLastLogTimestamp              = timestamp;
            mAmountOfLogsWithSameTimestamp = 0;
        }

        LogEntry entry;

        entry.timestamp    = timestamp + mAmountOfLogsWithSameTimestamp;
        entry.level        = level;
        entry.instrumentId = instrumentId;
        entry.message      = message;

        mInstrumentsStorage->lock();
        mLogosStorage->lock();

        const Instrument& instrument = mInstrumentsStorage->getInstruments().value(instrumentId);

        entry.instrumentLogo   = mLogosStorage->getLogo(instrumentId);
        entry.instrumentTicker = instrument.ticker;
        entry.instrumentName   = instrument.name;

        mInstrumentsStorage->unlock();
        mLogosStorage->unlock();

        mMutex->lock();
        mEntries.append(entry);
        mMutex->unlock();

        mSemaphore.release();
    }
}

void LogsThread::terminateThread()
{
    blockSignals(true);

    requestInterruption();

    mMutex->lock();
    mEntries.append(LogEntry());
    mMutex->unlock();

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
