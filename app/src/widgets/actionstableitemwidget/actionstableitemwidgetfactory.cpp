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
    Stock*                    stock,
    qint8                     precision,
    QWidget*                  parent
) const
{
    return new ActionsTableItemWidget(
        orderWavesDialogFactory, orderWavesWidgetFactory, orderBookThread, httpClient, stock, precision, parent
    );
}
