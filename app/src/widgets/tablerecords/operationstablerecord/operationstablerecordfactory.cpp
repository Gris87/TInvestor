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

IOperationsTableRecord* OperationsTableRecordFactory::newInstance(
    QTableWidget*                 tableWidget,
    IStockTableItemWidgetFactory* stockTableItemWidgetFactory,
    IUserStorage*                 userStorage,
    IInstrumentsStorage*          instrumentsStorage,
    QObject*                      parent
) const
{
    return new OperationsTableRecord(tableWidget, stockTableItemWidgetFactory, userStorage, instrumentsStorage, parent);
}
