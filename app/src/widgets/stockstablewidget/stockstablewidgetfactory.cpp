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
    IStocksTableModelFactory*       stocksTableModelFactory,
    IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*       orderWavesDialogFactory,
    IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
    IUserStorage*                   userStorage,
    IOrderBookThread*               orderBookThread,
    IHttpClient*                    httpClient,
    IFileDialogFactory*             fileDialogFactory,
    ISettingsEditor*                settingsEditor,
    QWidget*                        parent
) const
{
    return new StocksTableWidget(
        stocksTableModelFactory,
        actionsTableItemWidgetFactory,
        orderWavesDialogFactory,
        orderWavesWidgetFactory,
        userStorage,
        orderBookThread,
        httpClient,
        fileDialogFactory,
        settingsEditor,
        parent
    );
}
