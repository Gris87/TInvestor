#pragma once



#include "src/widgets/stockstablewidget/istockstablewidgetfactory.h"



class StocksTableWidgetFactory : public IStocksTableWidgetFactory
{
public:
    StocksTableWidgetFactory();
    ~StocksTableWidgetFactory();

    StocksTableWidgetFactory(const StocksTableWidgetFactory& another)            = delete;
    StocksTableWidgetFactory& operator=(const StocksTableWidgetFactory& another) = delete;

    IStocksTableWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) override;
};
