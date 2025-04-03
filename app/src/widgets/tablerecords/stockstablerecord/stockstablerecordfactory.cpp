#include "src/widgets/tablerecords/stockstablerecord/stockstablerecordfactory.h"

#include <QDebug>

#include "src/widgets/tablerecords/stockstablerecord/stockstablerecord.h"



TableRecordFactory::TableRecordFactory() :
    ITableRecordFactory()
{
    qDebug() << "Create TableRecordFactory";
}

TableRecordFactory::~TableRecordFactory()
{
    qDebug() << "Destroy TableRecordFactory";
}

ITableRecord* TableRecordFactory::newInstance(
    QTableWidget*                   tableWidget,
    IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
    IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*       orderWavesDialogFactory,
    IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
    IUserStorage*                   userStorage,
    IOrderBookThread*               orderBookThread,
    IHttpClient*                    httpClient,
    Stock*                          stock,
    QObject*                        parent
)
{
    return new TableRecord(
        tableWidget,
        stockTableItemWidgetFactory,
        actionsTableItemWidgetFactory,
        orderWavesDialogFactory,
        orderWavesWidgetFactory,
        userStorage,
        orderBookThread,
        httpClient,
        stock,
        parent
    );
}
