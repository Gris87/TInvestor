#pragma once



#include <QObject>

#include <QTableWidget>

#include "src/domain/filter/logfilter.h"
#include "src/qxlsx/xlsxdocument.h"



enum LogsTableOldColumns : qint8
{
    LOGS_OLD_TIME_COLUMN,
    LOGS_OLD_LEVEL_COLUMN,
    LOGS_OLD_NAME_COLUMN,
    LOGS_OLD_MESSAGE_COLUMN,
    LOGS_OLD_COLUMN_COUNT,
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

    virtual void setLogEntry(const LogEntry& entry)                         = 0;
    virtual void filter(QTableWidget* tableWidget, const LogFilter& filter) = 0;
    virtual void exportToExcel(QXlsx::Document& doc) const                  = 0;
};
