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
    QTableWidget*                      tableWidget,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    QObject*                           parent
) const
{
    return new OperationsTableRecord(tableWidget, instrumentTableItemWidgetFactory, userStorage, instrumentsStorage, parent);
}
