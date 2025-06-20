#pragma once



#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidget.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory.h"
#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecordfactory.h"



class IPortfolioTreeWidgetFactory
{
public:
    IPortfolioTreeWidgetFactory()          = default;
    virtual ~IPortfolioTreeWidgetFactory() = default;

    IPortfolioTreeWidgetFactory(const IPortfolioTreeWidgetFactory& another)            = delete;
    IPortfolioTreeWidgetFactory& operator=(const IPortfolioTreeWidgetFactory& another) = delete;

    virtual IPortfolioTreeWidget* newInstance(
        IPortfolioTreeModelFactory*  portfolioTreeModelFactory,
        ILogosStorage*               logosStorage,
        IPortfolioTreeRecordFactory* portfolioTreeRecordFactory,
        IInstrumentWidgetFactory*    instrumentWidgetFactory,
        IUserStorage*                userStorage,
        IInstrumentsStorage*         instrumentsStorage,
        IFileDialogFactory*          fileDialogFactory,
        ISettingsEditor*             settingsEditor,
        QWidget*                     parent
    ) const = 0;
};
