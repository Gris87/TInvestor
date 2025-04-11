#include "src/widgets/tableitems/stock/stocktableitemwidgetfactory.h"

#include <QDebug>

#include "src/widgets/tableitems/stock/stocktableitemwidget.h"



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
