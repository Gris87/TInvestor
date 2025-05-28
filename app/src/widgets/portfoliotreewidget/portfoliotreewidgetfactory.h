#pragma once



#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory.h"



class PortfolioTreeWidgetFactory : public IPortfolioTreeWidgetFactory
{
public:
    PortfolioTreeWidgetFactory();
    ~PortfolioTreeWidgetFactory() override;

    PortfolioTreeWidgetFactory(const PortfolioTreeWidgetFactory& another)            = delete;
    PortfolioTreeWidgetFactory& operator=(const PortfolioTreeWidgetFactory& another) = delete;

    IPortfolioTreeWidget* newInstance(
        IPortfolioTreeRecordFactory* portfolioTreeRecordFactory,
        IInstrumentsStorage*         instrumentsStorage,
        ISettingsEditor*             settingsEditor,
        QWidget*                     parent
    ) const override;
};
