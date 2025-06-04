#pragma once



#include "src/threads/logs/ilogsthread.h"

#include <QSemaphore>

#include "src/db/logs/ilogsdatabase.h"
#include "src/storage/user/iuserstorage.h"



class LogsThread : public ILogsThread
{
    Q_OBJECT

public:
    explicit LogsThread(IUserStorage* userStorage, ILogsDatabase* logsDatabase, QObject* parent = nullptr);
    ~LogsThread() override;

    LogsThread(const LogsThread& another)            = delete;
    LogsThread& operator=(const LogsThread& another) = delete;

    void run() override;

    void setAccount(const QString& account) override;
    void addLog(LogLevel level, const QString& message) override;
    void terminateThread() override;

private:
    void     readLogs();
    LogEntry takeEntry();

    QSemaphore      mSemaphore;
    QMutex*         mMutex;
    IUserStorage*   mUserStorage;
    ILogsDatabase*  mLogsDatabase;
    QString         mAccountId;
    QList<LogEntry> mEntries;
};
