#pragma once



#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory.h"



class LogsTableModelFactory : public ILogsTableModelFactory
{
public:
    LogsTableModelFactory();
    ~LogsTableModelFactory() override;

    LogsTableModelFactory(const LogsTableModelFactory& another)            = delete;
    LogsTableModelFactory& operator=(const LogsTableModelFactory& another) = delete;

    ILogsTableModel* newInstance(QObject* parent) const override;
};
