#pragma once



#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidget.h"
#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecordfactory.h"



class IPortfolioTreeWidgetFactory
{
public:
    IPortfolioTreeWidgetFactory()          = default;
    virtual ~IPortfolioTreeWidgetFactory() = default;

    IPortfolioTreeWidgetFactory(const IPortfolioTreeWidgetFactory& another)            = delete;
    IPortfolioTreeWidgetFactory& operator=(const IPortfolioTreeWidgetFactory& another) = delete;

    virtual IPortfolioTreeWidget* newInstance(
        IPortfolioTreeRecordFactory* portfolioTreeRecordFactory,
        IInstrumentWidgetFactory*    instrumentWidgetFactory,
        IUserStorage*                userStorage,
        IInstrumentsStorage*         instrumentsStorage,
        IFileDialogFactory*          fileDialogFactory,
        ISettingsEditor*             settingsEditor,
        QWidget*                     parent
    ) const = 0;
};
