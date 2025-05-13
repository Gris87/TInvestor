#include "src/widgets/tablerecords/operationstablerecord/operationstablerecord.h"

#include <QDebug>



OperationsTableRecord::OperationsTableRecord(QTableWidget* /*tableWidget*/, QObject* parent) :
    IOperationsTableRecord(parent)
{
    qDebug() << "Create OperationsTableRecord";
}

OperationsTableRecord::~OperationsTableRecord()
{
    qDebug() << "Destroy OperationsTableRecord";
}
