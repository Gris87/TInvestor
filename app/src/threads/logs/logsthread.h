#pragma once



#include "src/threads/logs/ilogsthread.h"

#include <QMutex>
#include <QSemaphore>

#include "src/db/logs/ilogsdatabase.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"



class LogsThread : public ILogsThread
{
    Q_OBJECT

public:
    explicit LogsThread(
        ILogsDatabase*       logsDatabase,
        IInstrumentsStorage* instrumentsStorage,
        ILogosStorage*       logosStorage,
        QObject*             parent = nullptr
    );
    ~LogsThread() override;

    LogsThread(const LogsThread& another)            = delete;
    LogsThread& operator=(const LogsThread& another) = delete;

    void run() override;

    void setAccountId(const QString& account, const QString& accountId) override;
    void addLog(LogLevel level, const QString& instrumentId, const QString& message) override;
    void terminateThread() override;

#ifdef TESTING_MODE
    void testTerminateWithoutTerminate();

    void testSetLimitLogs(int limitLogs)
    {
        mLimitLogs = limitLogs;
    }

    void testSetOptimizeSize(int optimizeSize)
    {
        mOptimizeSize = optimizeSize;
    }
#endif

    qint64 getTimestamp(qint64 timestamp);

private:
    void     readLogs();
    LogEntry takeIncomingEntry();
    void     optimize();

    QSemaphore           mSemaphore;
    QMutex*              mMutex;
    ILogsDatabase*       mLogsDatabase;
    IInstrumentsStorage* mInstrumentsStorage;
    ILogosStorage*       mLogosStorage;
    QString              mAccountId;
    qint64               mLastLogTimestamp;
    qint8                mAmountOfLogsWithSameTimestamp;
    QList<LogEntry>      mIncomingEntries;
    int                  mAmountOfEntries;
    int                  mLimitLogs;
    int                  mOptimizeSize;
};
