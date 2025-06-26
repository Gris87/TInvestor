#pragma once



#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidget.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory.h"



class IPortfolioTreeWidgetFactory
{
public:
    IPortfolioTreeWidgetFactory()          = default;
    virtual ~IPortfolioTreeWidgetFactory() = default;

    IPortfolioTreeWidgetFactory(const IPortfolioTreeWidgetFactory& another)            = delete;
    IPortfolioTreeWidgetFactory& operator=(const IPortfolioTreeWidgetFactory& another) = delete;

    virtual IPortfolioTreeWidget* newInstance(
        IPortfolioTreeModelFactory* portfolioTreeModelFactory,
        IFileDialogFactory*         fileDialogFactory,
        ISettingsEditor*            settingsEditor,
        QWidget*                    parent
    ) const = 0;
};
