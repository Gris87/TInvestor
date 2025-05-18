#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/fs/file/ifilefactory.h"
#include "src/utils/messagebox/imessageboxutils.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"
#include "src/widgets/portfoliotablewidget/iportfoliotablewidgetfactory.h"



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
        IPortfolioTableWidgetFactory*      portfolioTableWidgetFactory,
        IOperationsTableRecordFactory*     operationsTableRecordFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IFileFactory*                      fileFactory,
        IFileDialogFactory*                fileDialogFactory,
        IMessageBoxUtils*                  messageBoxUtils,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent
    ) const = 0;
};
