#pragma once



#include "src/widgets/tablerecord/items/stock/istocktableitemwidgetfactory.h"

#include <gmock/gmock.h>



class StockTableItemWidgetFactoryMock : public IStockTableItemWidgetFactory
{
public:
    StockTableItemWidgetFactoryMock() :
        IStockTableItemWidgetFactory()
    {
    }
    ~StockTableItemWidgetFactoryMock() override = default;

    StockTableItemWidgetFactoryMock(const StockTableItemWidgetFactoryMock& another)            = delete;
    StockTableItemWidgetFactoryMock& operator=(const StockTableItemWidgetFactoryMock& another) = delete;

    MOCK_METHOD(IStockTableItemWidget*, newInstance, (IUserStorage * userStorage, QWidget* parent), (override));
};
