#pragma once



#include "src/widgets/tableitems/stock/istocktableitemwidget.h"

#include "src/storage/user/iuserstorage.h"



class IStockTableItemWidgetFactory
{
public:
    IStockTableItemWidgetFactory()          = default;
    virtual ~IStockTableItemWidgetFactory() = default;

    IStockTableItemWidgetFactory(const IStockTableItemWidgetFactory& another)            = delete;
    IStockTableItemWidgetFactory& operator=(const IStockTableItemWidgetFactory& another) = delete;

    virtual IStockTableItemWidget* newInstance(IUserStorage* userStorage, QWidget* parent) const = 0;
};
