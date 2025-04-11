#pragma once



#include "src/widgets/stockscontrolswidget/istockscontrolswidgetfactory.h"



class StocksControlsWidgetFactory : public IStocksControlsWidgetFactory
{
public:
    StocksControlsWidgetFactory();
    ~StocksControlsWidgetFactory();

    StocksControlsWidgetFactory(const StocksControlsWidgetFactory& another)            = delete;
    StocksControlsWidgetFactory& operator=(const StocksControlsWidgetFactory& another) = delete;

    IStocksControlsWidget*
    newInstance(IStocksStorage* stocksStorage, ISettingsEditor* settingsEditor, QWidget* parent) const override;
};
