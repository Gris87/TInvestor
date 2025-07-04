#pragma once



#include "src/widgets/stockscontrolswidget/istockscontrolswidgetfactory.h"



class StocksControlsWidgetFactory : public IStocksControlsWidgetFactory
{
public:
    StocksControlsWidgetFactory();
    ~StocksControlsWidgetFactory() override;

    StocksControlsWidgetFactory(const StocksControlsWidgetFactory& another)            = delete;
    StocksControlsWidgetFactory& operator=(const StocksControlsWidgetFactory& another) = delete;

    IStocksControlsWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const override;
};
