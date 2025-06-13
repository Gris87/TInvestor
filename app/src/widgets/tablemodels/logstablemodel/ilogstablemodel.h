#pragma once



#include <QAbstractTableModel>

#include "src/domain/log/logentry.h"



enum LogsTableColumns : qint8
{
    LOGS_TIME_COLUMN,
    LOGS_LEVEL_COLUMN,
    LOGS_NAME_COLUMN,
    LOGS_MESSAGE_COLUMN,
    LOGS_COLUMN_COUNT,
};

enum LogsTableUserRoles : qint16
{
    LOGS_ROLE_INSTRUMENT_LOGO = Qt::UserRole + 1,
    LOGS_ROLE_INSTRUMENT_NAME = Qt::UserRole + 2
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

    virtual void logsRead(const QList<LogEntry>& entries) = 0;
    virtual void logAdded(const LogEntry& entry)          = 0;
};
