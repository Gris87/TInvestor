#pragma once



#include <QTableWidget>

#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecord.h"



class IOperationsTableRecordFactory
{
public:
    IOperationsTableRecordFactory()          = default;
    virtual ~IOperationsTableRecordFactory() = default;

    IOperationsTableRecordFactory(const IOperationsTableRecordFactory& another)            = delete;
    IOperationsTableRecordFactory& operator=(const IOperationsTableRecordFactory& another) = delete;

    virtual IOperationsTableRecord* newInstance(QTableWidget* tableWidget, QObject* parent) const = 0;
};
