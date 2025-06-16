#pragma once



#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory.h"



class OperationsTableModelFactory : public IOperationsTableModelFactory
{
public:
    OperationsTableModelFactory();
    ~OperationsTableModelFactory() override;

    OperationsTableModelFactory(const OperationsTableModelFactory& another)            = delete;
    OperationsTableModelFactory& operator=(const OperationsTableModelFactory& another) = delete;

    IOperationsTableModel* newInstance(QObject* parent) const override;
};
