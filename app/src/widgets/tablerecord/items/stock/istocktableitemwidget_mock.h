#pragma once



#include "src/widgets/tablerecord/items/stock/istocktableitemwidget.h"

#include <gmock/gmock.h>



class StockTableItemWidgetMock : public IStockTableItemWidget
{
public:
    explicit StockTableItemWidgetMock(QWidget* parent = nullptr) :
        IStockTableItemWidget(parent)
    {
    }
    ~StockTableItemWidgetMock() override = default;

    StockTableItemWidgetMock(const StockTableItemWidgetMock& another)            = delete;
    StockTableItemWidgetMock& operator=(const StockTableItemWidgetMock& another) = delete;
};
