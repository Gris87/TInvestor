#pragma once



#include "src/widgets/portfoliotablewidget/iportfoliotablewidgetfactory.h"



class PortfolioTableWidgetFactory : public IPortfolioTableWidgetFactory
{
public:
    PortfolioTableWidgetFactory();
    ~PortfolioTableWidgetFactory() override;

    PortfolioTableWidgetFactory(const PortfolioTableWidgetFactory& another)            = delete;
    PortfolioTableWidgetFactory& operator=(const PortfolioTableWidgetFactory& another) = delete;

    IPortfolioTableWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const override;
};
