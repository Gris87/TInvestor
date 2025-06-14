#pragma once



#include <QThread>

#include "src/domain/log/logentry.h"



class ILogsThread : public QThread
{
    Q_OBJECT

public:
    explicit ILogsThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~ILogsThread() override = default;

    ILogsThread(const ILogsThread& another)            = delete;
    ILogsThread& operator=(const ILogsThread& another) = delete;

    virtual void setAccountId(const QString& account, const QString& accountId)              = 0;
    virtual void addLog(LogLevel level, const QString& instrumentId, const QString& message) = 0;
    virtual void terminateThread()                                                           = 0;

signals:
    void logsRead(const QList<LogEntry>& entries);
    void logAdded(const LogEntry& entry);
};
