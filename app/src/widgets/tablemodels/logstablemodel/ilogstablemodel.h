#pragma once



#include <QAbstractTableModel>

#include "src/domain/filter/logfilter.h"
#include "src/domain/log/logentry.h"
#include "src/qxlsx/xlsxdocument.h"



enum LogsTableColumns : qint8
{
    LOGS_TIME_COLUMN,
    LOGS_LEVEL_COLUMN,
    LOGS_NAME_COLUMN,
    LOGS_MESSAGE_COLUMN,
    LOGS_COLUMN_COUNT,
};



class ILogsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ILogsTableModel(QObject* parent = nullptr) :
        QAbstractTableModel(parent)
    {
    }
    ~ILogsTableModel() override = default;

    ILogsTableModel(const ILogsTableModel& another)            = delete;
    ILogsTableModel& operator=(const ILogsTableModel& another) = delete;

    virtual void setFilter(const LogFilter& filter)        = 0;
    virtual void logsRead(const QList<LogEntry>& entries)  = 0;
    virtual void logAdded(const LogEntry& entry)           = 0;
    virtual void exportToExcel(QXlsx::Document& doc) const = 0;
};
