#pragma once



#include "src/widgets/portfoliotreewidget/iportfoliotreewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



class IPortfolioTreeWidgetFactory
{
public:
    IPortfolioTreeWidgetFactory()          = default;
    virtual ~IPortfolioTreeWidgetFactory() = default;

    IPortfolioTreeWidgetFactory(const IPortfolioTreeWidgetFactory& another)            = delete;
    IPortfolioTreeWidgetFactory& operator=(const IPortfolioTreeWidgetFactory& another) = delete;

    virtual IPortfolioTreeWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const = 0;
};
