#pragma once



#include "src/widgets/stockstablewidget/istockstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



class IStocksTableWidgetFactory
{
public:
    IStocksTableWidgetFactory()
    {
    }
    virtual ~IStocksTableWidgetFactory() = default;

    IStocksTableWidgetFactory(const IStocksTableWidgetFactory& another)            = delete;
    IStocksTableWidgetFactory& operator=(const IStocksTableWidgetFactory& another) = delete;

    virtual IStocksTableWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) = 0;
};
