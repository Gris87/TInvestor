#pragma once



#include "src/widgets/tablerecords/logstablerecord/ilogstablerecord.h"

#include <gmock/gmock.h>



class LogsTableRecordMock : public ILogsTableRecord
{
    Q_OBJECT

public:
    explicit LogsTableRecordMock(QObject* parent = nullptr) :
        ILogsTableRecord(parent)
    {
    }
    ~LogsTableRecordMock() override = default;

    LogsTableRecordMock(const LogsTableRecordMock& another)            = delete;
    LogsTableRecordMock& operator=(const LogsTableRecordMock& another) = delete;

    MOCK_METHOD(void, setLogEntry, (const LogEntry& entry), (override));
    MOCK_METHOD(void, filter, (QTableWidget * tableWidget, const LogFilter& filter), (override));
    MOCK_METHOD(void, exportToExcel, (QXlsx::Document & doc), (const, override));
};
