#pragma once



#include "src/widgets/tablerecord/items/stock/istocktableitemwidgetfactory.h"



class StockTableItemWidgetFactory : public IStockTableItemWidgetFactory
{
public:
    StockTableItemWidgetFactory();
    ~StockTableItemWidgetFactory();

    StockTableItemWidgetFactory(const StockTableItemWidgetFactory& another)            = delete;
    StockTableItemWidgetFactory& operator=(const StockTableItemWidgetFactory& another) = delete;

    IStockTableItemWidget* newInstance(QWidget* parent) override;
};
