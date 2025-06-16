#pragma once



#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodel.h"



class IOperationsTableModelFactory
{
public:
    IOperationsTableModelFactory()          = default;
    virtual ~IOperationsTableModelFactory() = default;

    IOperationsTableModelFactory(const IOperationsTableModelFactory& another)            = delete;
    IOperationsTableModelFactory& operator=(const IOperationsTableModelFactory& another) = delete;

    virtual IOperationsTableModel* newInstance(QObject* parent) const = 0;
};
