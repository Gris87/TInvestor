#include "src/threads/logs/logsthread.h"

#include <QDebug>

#include "src/threads/parallelhelper/parallelhelperthread.h"



constexpr int LIMIT_LOGS    = 1000000;
constexpr int OPTIMIZE_SIZE = 100000;



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
    mIncomingEntries(),
    mAmountOfEntries(),
    mLimitLogs(LIMIT_LOGS),
    mOptimizeSize(OPTIMIZE_SIZE)
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
        if (mAmountOfEntries > mLimitLogs)
        {
            optimize();
        }

        mSemaphore.acquire();

        const LogEntry entry = takeIncomingEntry();

        if (entry.timestamp == 0)
        {
            break;
        }

        emit logAdded(entry);

        mLogsDatabase->appendLog(entry);

        ++mAmountOfEntries;
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

        entry.timestamp    = getTimestamp(QDateTime::currentMSecsSinceEpoch());
        entry.level        = level;
        entry.instrumentId = instrumentId;
        entry.message      = message;

        if (instrumentId != "")
        {
            mInstrumentsStorage->readLock();
            Instrument instrument = mInstrumentsStorage->getInstruments().value(instrumentId);
            mInstrumentsStorage->readUnlock();

            if (instrument.ticker == "" || instrument.name == "")
            {
                instrument.ticker         = instrumentId;
                instrument.name           = "?????";
                instrument.pricePrecision = 2;
            }

            mLogosStorage->readLock();
            entry.instrumentLogo = mLogosStorage->getLogo(instrumentId);
            mLogosStorage->readUnlock();

            entry.instrumentTicker = instrument.ticker;
            entry.instrumentName   = instrument.name;
        }

        mMutex->lock();
        mIncomingEntries.append(entry);
        mMutex->unlock();

        mSemaphore.release();
    }
}

void LogsThread::terminateThread()
{
    if (isRunning())
    {
        blockSignals(true);

        requestInterruption();

        mMutex->lock();
        mIncomingEntries.append(LogEntry());
        mMutex->unlock();

        mSemaphore.release();
    }
}

#ifdef TESTING_MODE
void LogsThread::testTerminateWithoutTerminate()
{
    mMutex->lock();
    mIncomingEntries.append(LogEntry());
    mMutex->unlock();

    mSemaphore.release();
}
#endif

qint64 LogsThread::getTimestamp(qint64 timestamp)
{
    if (timestamp == mLastLogTimestamp)
    {
        ++mAmountOfLogsWithSameTimestamp;
    }
    else
    {
        mLastLogTimestamp              = timestamp;
        mAmountOfLogsWithSameTimestamp = 0;
    }

    return timestamp + mAmountOfLogsWithSameTimestamp;
}

void LogsThread::readLogs()
{
    const QList<LogEntry> entries = mLogsDatabase->readLogs();
    mAmountOfEntries              = entries.size();

    emit logsRead(entries);
}

LogEntry LogsThread::takeIncomingEntry()
{
    const QMutexLocker lock(mMutex);

    return mIncomingEntries.takeFirst();
}

struct OptimizeLogsInfo
{
    explicit OptimizeLogsInfo(const QList<LogEntry>* _entries) :
        entries(_entries)
    {
    }

    const QList<LogEntry>* entries;
};

static void
optimizeLogsForParallel(QThread* parentThread, int /*threadId*/, QList<LogEntry>& res, int start, int end, void* additionalArgs)
{
    OptimizeLogsInfo* optimizeLogsInfo = reinterpret_cast<OptimizeLogsInfo*>(additionalArgs);

    const LogEntry* entriesArray = optimizeLogsInfo->entries->data();

    LogEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[i];
    }
}

void LogsThread::optimize()
{
    const QList<LogEntry> entries = mLogsDatabase->readLogs();

    QList<LogEntry> newEntries;
    newEntries.resizeForOverwrite(mOptimizeSize);

    OptimizeLogsInfo optimizeLogsInfo(&entries);
    processInParallel(newEntries, optimizeLogsForParallel, &optimizeLogsInfo);

    mAmountOfEntries = newEntries.size();

    mLogsDatabase->writeLogs(newEntries);
    emit logsRead(newEntries);
}
