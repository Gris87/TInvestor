#pragma once



#include "src/widgets/tablerecord/items/stock/istocktableitemwidget.h"

#include "src/domain/stocks/stock.h"



class IStockTableItemWidgetFactory
{
public:
    IStockTableItemWidgetFactory()
    {
    }
    virtual ~IStockTableItemWidgetFactory() = default;

    IStockTableItemWidgetFactory(const IStockTableItemWidgetFactory& another)            = delete;
    IStockTableItemWidgetFactory& operator=(const IStockTableItemWidgetFactory& another) = delete;

    virtual IStockTableItemWidget* newInstance(QWidget* parent) = 0;
};
