#include "src/widgets/actionstableitemwidget/actionstableitemwidgetfactory.h"

#include <QDebug>

#include "src/widgets/actionstableitemwidget/actionstableitemwidget.h"



ActionsTableItemWidgetFactory::ActionsTableItemWidgetFactory() :
    IActionsTableItemWidgetFactory()
{
    qDebug() << "Create ActionsTableItemWidgetFactory";
}

ActionsTableItemWidgetFactory::~ActionsTableItemWidgetFactory()
{
    qDebug() << "Destroy ActionsTableItemWidgetFactory";
}

IActionsTableItemWidget* ActionsTableItemWidgetFactory::newInstance(
    IOrderWavesDialogFactory* orderWavesDialogFactory,
    IOrderWavesWidgetFactory* orderWavesWidgetFactory,
    IOrderBookThread*         orderBookThread,
    IHttpClient*              httpClient,
    IStocksTableModel*        stocksTableModel,
    int                       tableRow,
    QWidget*                  parent
) const
{
    return new ActionsTableItemWidget(
        orderWavesDialogFactory, orderWavesWidgetFactory, orderBookThread, httpClient, stocksTableModel, tableRow, parent
    );
}
