#include "src/widgets/tablerecords/operationstablerecord/operationstablerecordfactory.h"

#include <QDebug>

#include "src/widgets/tablerecords/operationstablerecord/operationstablerecord.h"



OperationsTableRecordFactory::OperationsTableRecordFactory() :
    IOperationsTableRecordFactory()
{
    qDebug() << "Create OperationsTableRecordFactory";
}

OperationsTableRecordFactory::~OperationsTableRecordFactory()
{
    qDebug() << "Destroy OperationsTableRecordFactory";
}

IOperationsTableRecord* OperationsTableRecordFactory::newInstance(QTableWidget* tableWidget, QObject* parent) const
{
    return new OperationsTableRecord(tableWidget, parent);
}
