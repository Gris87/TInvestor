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
    IStocksTableModelFactory*          stocksTableModelFactory,
    IStocksTableRecordFactory*         stockTableRecordFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*          orderWavesDialogFactory,
    IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
    ILogosStorage*                     logosStorage,
    IUserStorage*                      userStorage,
    IOrderBookThread*                  orderBookThread,
    IHttpClient*                       httpClient,
    IFileDialogFactory*                fileDialogFactory,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) const
{
    return new StocksTableWidget(
        stocksTableModelFactory,
        stockTableRecordFactory,
        instrumentTableItemWidgetFactory,
        actionsTableItemWidgetFactory,
        orderWavesDialogFactory,
        orderWavesWidgetFactory,
        logosStorage,
        userStorage,
        orderBookThread,
        httpClient,
        fileDialogFactory,
        settingsEditor,
        parent
    );
}
