#include "src/widgets/stockstablewidget/stockstablewidgetfactory.h"

#include <QDebug>

#include "src/widgets/stockstablewidget/stockstablewidget.h"



StocksTableWidgetFactory::StocksTableWidgetFactory() :
    IStocksTableWidgetFactory()
{
    qDebug() << "Create StocksTableWidgetFactory";
}

StocksTableWidgetFactory::~StocksTableWidgetFactory()
{
    qDebug() << "Destroy StocksTableWidgetFactory";
}

IStocksTableWidget* StocksTableWidgetFactory::newInstance(
    IStocksTableRecordFactory*         stockTableRecordFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*          orderWavesDialogFactory,
    IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
    IUserStorage*                      userStorage,
    IOrderBookThread*                  orderBookThread,
    IHttpClient*                       httpClient,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) const
{
    return new StocksTableWidget(
        stockTableRecordFactory,
        instrumentTableItemWidgetFactory,
        actionsTableItemWidgetFactory,
        orderWavesDialogFactory,
        orderWavesWidgetFactory,
        userStorage,
        orderBookThread,
        httpClient,
        settingsEditor,
        parent
    );
}
