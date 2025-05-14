#include "src/widgets/tablerecords/stockstablerecord/stockstablerecordfactory.h"

#include <QDebug>

#include "src/widgets/tablerecords/stockstablerecord/stockstablerecord.h"



StocksTableRecordFactory::StocksTableRecordFactory() :
    IStocksTableRecordFactory()
{
    qDebug() << "Create StocksTableRecordFactory";
}

StocksTableRecordFactory::~StocksTableRecordFactory()
{
    qDebug() << "Destroy StocksTableRecordFactory";
}

IStocksTableRecord* StocksTableRecordFactory::newInstance(
    QTableWidget*                      tableWidget,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*          orderWavesDialogFactory,
    IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
    IUserStorage*                      userStorage,
    IOrderBookThread*                  orderBookThread,
    IHttpClient*                       httpClient,
    Stock*                             stock,
    QObject*                           parent
) const
{
    return new StocksTableRecord(
        tableWidget,
        instrumentTableItemWidgetFactory,
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
