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

    virtual void setAccount(const QString& account)             = 0;
    virtual void addLog(LogLevel level, const QString& message) = 0;
    virtual void terminateThread()                              = 0;

signals:
    void accountNotFound();
    void logsRead(const QList<LogEntry>& entries);
    void logAdded(const LogEntry& entry);
};
