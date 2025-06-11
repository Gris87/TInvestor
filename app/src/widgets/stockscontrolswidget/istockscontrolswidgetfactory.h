#pragma once



#include "src/widgets/stockscontrolswidget/istockscontrolswidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



class IStocksControlsWidgetFactory
{
public:
    IStocksControlsWidgetFactory()          = default;
    virtual ~IStocksControlsWidgetFactory() = default;

    IStocksControlsWidgetFactory(const IStocksControlsWidgetFactory& another)            = delete;
    IStocksControlsWidgetFactory& operator=(const IStocksControlsWidgetFactory& another) = delete;

    virtual IStocksControlsWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const = 0;
};
