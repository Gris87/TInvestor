#pragma once



#include "src/widgets/stockscontrolswidget/istockscontrolswidget.h"

#include "src/storage/stocks/istocksstorage.h"
#include "src/utils/settingseditor/isettingseditor.h"



class IStocksControlsWidgetFactory
{
public:
    IStocksControlsWidgetFactory()
    {
    }
    virtual ~IStocksControlsWidgetFactory() = default;

    IStocksControlsWidgetFactory(const IStocksControlsWidgetFactory& another)            = delete;
    IStocksControlsWidgetFactory& operator=(const IStocksControlsWidgetFactory& another) = delete;

    virtual IStocksControlsWidget*
    newInstance(IStocksStorage* stocksStorage, ISettingsEditor* settingsEditor, QWidget* parent) = 0;
};
