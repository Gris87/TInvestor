#pragma once



#include "src/widgets/tablerecord/items/stock/istocktableitemwidget.h"

#include "src/domain/stocks/stock.h"



namespace Ui
{
class StockTableItemWidget;
}



class StockTableItemWidget : public IStockTableItemWidget
{
    Q_OBJECT

public:
    explicit StockTableItemWidget(QWidget* parent = nullptr);
    ~StockTableItemWidget();

    StockTableItemWidget(const StockTableItemWidget& another)            = delete;
    StockTableItemWidget& operator=(const StockTableItemWidget& another) = delete;

    Ui::StockTableItemWidget* ui;
};
