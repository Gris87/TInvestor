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
        IPortfolioTreeModelFactory* portfolioTreeModelFactory,
        IFileDialogFactory*         fileDialogFactory,
        IMessageBoxUtils*           messageBoxUtils,
        ISettingsEditor*            settingsEditor,
        bool                        autoPilot,
        QWidget*                    parent
    ) const override;
};
