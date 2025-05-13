#include "src/widgets/tablerecords/operationstablerecord/operationstablerecord.h"

#include <QDebug>



OperationsTableRecord::OperationsTableRecord(QTableWidget* tableWidget, QObject* parent) :
    IOperationsTableRecord(parent),
    mTimeTableWidgetItem(new TimeTableItem())
{
    qDebug() << "Create OperationsTableRecord";

    const int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    tableWidget->setItem(rowIndex, OPERATIONS_TIME_COLUMN, mTimeTableWidgetItem);
}

OperationsTableRecord::~OperationsTableRecord()
{
    qDebug() << "Destroy OperationsTableRecord";
}

void OperationsTableRecord::setOperation(const Operation& operation)
{
    mTimeTableWidgetItem->setValue(QDateTime::fromMSecsSinceEpoch(operation.timestamp));
}
