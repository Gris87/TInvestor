#pragma once



#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory.h"



class LogsTableRecordFactory : public ILogsTableRecordFactory
{
public:
    LogsTableRecordFactory();
    ~LogsTableRecordFactory() override;

    LogsTableRecordFactory(const LogsTableRecordFactory& another)            = delete;
    LogsTableRecordFactory& operator=(const LogsTableRecordFactory& another) = delete;

    ILogsTableRecord* newInstance(QTableWidget* tableWidget, QObject* parent) const override;
};
