#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory.h"



class IDecisionMakerWidgetFactory
{
public:
    IDecisionMakerWidgetFactory()          = default;
    virtual ~IDecisionMakerWidgetFactory() = default;

    IDecisionMakerWidgetFactory(const IDecisionMakerWidgetFactory& another)            = delete;
    IDecisionMakerWidgetFactory& operator=(const IDecisionMakerWidgetFactory& another) = delete;

    virtual IDecisionMakerWidget* newInstance(
        IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
        IAccountChartWidgetFactory*        accountChartWidgetFactory,
        ILogsTableWidgetFactory*           logsTableWidgetFactory,
        IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
        IOperationsTableRecordFactory*     operationsTableRecordFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent
    ) const = 0;
};
