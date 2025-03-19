#include "src/widgets/tablerecord/items/actions/actionstableitemwidgetfactory.h"

#include <QDebug>

#include "src/widgets/tablerecord/items/actions/actionstableitemwidget.h"



ActionsTableItemWidgetFactory::ActionsTableItemWidgetFactory() :
    IActionsTableItemWidgetFactory()
{
    qDebug() << "Create ActionsTableItemWidgetFactory";
}

ActionsTableItemWidgetFactory::~ActionsTableItemWidgetFactory()
{
    qDebug() << "Destroy ActionsTableItemWidgetFactory";
}

IActionsTableItemWidget* ActionsTableItemWidgetFactory::newInstance(IHttpClient* httpClient, Stock* stock, QWidget* parent)
{
    return new ActionsTableItemWidget(httpClient, stock, parent);
}
