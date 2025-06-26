#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory.h"
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
        IOperationsTableWidgetFactory* operationsTableWidgetFactory,
        IAccountChartWidgetFactory*    accountChartWidgetFactory,
        ILogsFilterWidgetFactory*      logsFilterWidgetFactory,
        ILogsTableWidgetFactory*       logsTableWidgetFactory,
        IPortfolioTreeWidgetFactory*   portfolioTreeWidgetFactory,
        IOperationsTableModelFactory*  operationsTableModelFactory,
        ILogsTableModelFactory*        logsTableModelFactory,
        IPortfolioTreeModelFactory*    portfolioTreeModelFactory,
        IFileDialogFactory*            fileDialogFactory,
        ISettingsEditor*               settingsEditor,
        QWidget*                       parent
    ) const = 0;
};
