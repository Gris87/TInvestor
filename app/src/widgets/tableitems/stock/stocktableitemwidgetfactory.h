#pragma once



#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory.h"



class StockTableItemWidgetFactory : public IStockTableItemWidgetFactory
{
public:
    StockTableItemWidgetFactory();
    ~StockTableItemWidgetFactory() override;

    StockTableItemWidgetFactory(const StockTableItemWidgetFactory& another)            = delete;
    StockTableItemWidgetFactory& operator=(const StockTableItemWidgetFactory& another) = delete;

    IStockTableItemWidget* newInstance(IUserStorage* userStorage, QWidget* parent) const override;
};
