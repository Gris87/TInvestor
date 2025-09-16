#pragma once



#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodel.h"

#include "src/config/iconfig.h"



class IOperationsTableModelFactory
{
public:
    IOperationsTableModelFactory()          = default;
    virtual ~IOperationsTableModelFactory() = default;

    IOperationsTableModelFactory(const IOperationsTableModelFactory& another)            = delete;
    IOperationsTableModelFactory& operator=(const IOperationsTableModelFactory& another) = delete;

    virtual IOperationsTableModel* newInstance(IConfig* config, QObject* parent) const = 0;
};
