#include "src/widgets/tableitems/actions/actionstableitemwidgetfactory.h"

#include <QDebug>

#include "src/widgets/tableitems/actions/actionstableitemwidget.h"



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
    Stock*                    stock,
    int                       precision,
    QWidget*                  parent
)
{
    return new ActionsTableItemWidget(
        orderWavesDialogFactory, orderWavesWidgetFactory, orderBookThread, httpClient, stock, precision, parent
    );
}
