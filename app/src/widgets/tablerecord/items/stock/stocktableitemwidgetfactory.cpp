#include "src/widgets/tablerecord/items/stock/stocktableitemwidgetfactory.h"

#include <QDebug>

#include "src/widgets/tablerecord/items/stock/stocktableitemwidget.h"



StockTableItemWidgetFactory::StockTableItemWidgetFactory() :
    IStockTableItemWidgetFactory()
{
    qDebug() << "Create StockTableItemWidgetFactory";
}

StockTableItemWidgetFactory::~StockTableItemWidgetFactory()
{
    qDebug() << "Destroy StockTableItemWidgetFactory";
}

IStockTableItemWidget* StockTableItemWidgetFactory::newInstance(QWidget* parent)
{
    return new StockTableItemWidget(parent);
}
