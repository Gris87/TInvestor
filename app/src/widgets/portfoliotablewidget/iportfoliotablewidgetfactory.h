#pragma once



#include "src/widgets/portfoliotablewidget/iportfoliotablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



class IPortfolioTableWidgetFactory
{
public:
    IPortfolioTableWidgetFactory()
    {
    }
    virtual ~IPortfolioTableWidgetFactory() = default;

    IPortfolioTableWidgetFactory(const IPortfolioTableWidgetFactory& another)            = delete;
    IPortfolioTableWidgetFactory& operator=(const IPortfolioTableWidgetFactory& another) = delete;

    virtual IPortfolioTableWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) = 0;
};
