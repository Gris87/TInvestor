#pragma once



#include "src/widgets/filterwidget/ifilterwidget.h"

#include "src/storage/stocks/istocksstorage.h"



class IFilterWidgetFactory
{
public:
    IFilterWidgetFactory()
    {
    }
    virtual ~IFilterWidgetFactory() = default;

    IFilterWidgetFactory(const IFilterWidgetFactory& another)            = delete;
    IFilterWidgetFactory& operator=(const IFilterWidgetFactory& another) = delete;

    virtual IFilterWidget* newInstance(IStocksStorage* stocksStorage, QWidget* parent) = 0;
};
