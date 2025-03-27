#include "src/widgets/tablerecord/tablerecordfactory.h"

#include <QDebug>

#include "src/widgets/tablerecord/tablerecord.h"



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
        orderBookThread,
        httpClient,
        stock,
        parent
    );
}
