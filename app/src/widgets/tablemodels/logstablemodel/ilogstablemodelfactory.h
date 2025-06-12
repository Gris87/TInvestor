#pragma once



#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel.h"



class ILogsTableModelFactory
{
public:
    ILogsTableModelFactory()          = default;
    virtual ~ILogsTableModelFactory() = default;

    ILogsTableModelFactory(const ILogsTableModelFactory& another)            = delete;
    ILogsTableModelFactory& operator=(const ILogsTableModelFactory& another) = delete;

    virtual ILogsTableModel* newInstance(QObject* parent) const = 0;
};
