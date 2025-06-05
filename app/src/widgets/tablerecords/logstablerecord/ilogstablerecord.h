#pragma once



#include <QObject>

#include <QTableWidget>

#include "src/domain/log/logentry.h"
#include "src/qxlsx/xlsxdocument.h"



enum LogsTableColumns : qint8
{
    LOGS_TIME_COLUMN,
    LOGS_LEVEL_COLUMN,
    LOGS_MESSAGE_COLUMN,
    LOGS_COLUMN_COUNT,
};



class ILogsTableRecord : public QObject
{
    Q_OBJECT

public:
    explicit ILogsTableRecord(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    ~ILogsTableRecord() override = default;

    ILogsTableRecord(const ILogsTableRecord& another)            = delete;
    ILogsTableRecord& operator=(const ILogsTableRecord& another) = delete;

    virtual void setLogEntry(const LogEntry& entry)                = 0;
    virtual void filter(QTableWidget* tableWidget, LogLevel level) = 0;
    virtual void exportToExcel(QXlsx::Document& doc) const         = 0;
};
