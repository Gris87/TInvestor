#include "src/widgets/tableitems/instrument/instrumenttableitemwidgetfactory.h"

#include <QDebug>

#include "src/widgets/tableitems/instrument/instrumenttableitemwidget.h"



StockTableItemWidgetFactory::StockTableItemWidgetFactory() :
    IStockTableItemWidgetFactory()
{
    qDebug() << "Create StockTableItemWidgetFactory";
}

StockTableItemWidgetFactory::~StockTableItemWidgetFactory()
{
    qDebug() << "Destroy StockTableItemWidgetFactory";
}

IStockTableItemWidget* StockTableItemWidgetFactory::newInstance(IUserStorage* userStorage, QWidget* parent) const
{
    return new StockTableItemWidget(userStorage, parent);
}
