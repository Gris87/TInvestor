#pragma once



#include "src/widgets/stockscontrolswidget/istockscontrolswidget.h"

#include "src/storage/stocks/istocksstorage.h"



class IStocksControlsWidgetFactory
{
public:
    IStocksControlsWidgetFactory()
    {
    }
    virtual ~IStocksControlsWidgetFactory() = default;

    IStocksControlsWidgetFactory(const IStocksControlsWidgetFactory& another)            = delete;
    IStocksControlsWidgetFactory& operator=(const IStocksControlsWidgetFactory& another) = delete;

    virtual IStocksControlsWidget* newInstance(IStocksStorage* stocksStorage, QWidget* parent) = 0;
};
