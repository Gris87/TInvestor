#pragma once



#include <QWidget>

#include "src/domain/filter/logfilter.h"
#include "src/domain/log/logentry.h"



class ILogsTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ILogsTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~ILogsTableWidget() override = default;

    ILogsTableWidget(const ILogsTableWidget& another)            = delete;
    ILogsTableWidget& operator=(const ILogsTableWidget& another) = delete;

    virtual void setFilter(const LogFilter& filter)                                = 0;
    virtual void logsRead(const QList<LogEntry>& entries, const LogFilter& filter) = 0;
    virtual void logAdded(const LogEntry& entry, const LogFilter& filter)          = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
