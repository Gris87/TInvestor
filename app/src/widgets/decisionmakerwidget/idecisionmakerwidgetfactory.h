#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory.h"



struct DecisionMakerWidgetFactoryNewInstanceArgsMore15
{
    explicit DecisionMakerWidgetFactoryNewInstanceArgsMore15(
        IOperationsTableWidgetFactory* _operationsTableWidgetFactory,
        IAccountChartWidgetFactory*    _accountChartWidgetFactory,
        ILogsFilterWidgetFactory*      _logsFilterWidgetFactory,
        ILogsTableWidgetFactory*       _logsTableWidgetFactory,
        IPortfolioTreeWidgetFactory*   _portfolioTreeWidgetFactory,
        IOperationsTableModelFactory*  _operationsTableModelFactory,
        ILogsTableModelFactory*        _logsTableModelFactory,
        IPortfolioTreeModelFactory*    _portfolioTreeModelFactory,
        IPortfolioTreeRecordFactory*   _portfolioTreeRecordFactory,
        IInstrumentWidgetFactory*      _instrumentWidgetFactory,
        IUserStorage*                  _userStorage,
        IInstrumentsStorage*           _instrumentsStorage,
        ILogosStorage*                 _logosStorage,
        IFileDialogFactory*            _fileDialogFactory,
        ISettingsEditor*               _settingsEditor
    ) :
        operationsTableWidgetFactory(_operationsTableWidgetFactory),
        accountChartWidgetFactory(_accountChartWidgetFactory),
        logsFilterWidgetFactory(_logsFilterWidgetFactory),
        logsTableWidgetFactory(_logsTableWidgetFactory),
        portfolioTreeWidgetFactory(_portfolioTreeWidgetFactory),
        operationsTableModelFactory(_operationsTableModelFactory),
        logsTableModelFactory(_logsTableModelFactory),
        portfolioTreeModelFactory(_portfolioTreeModelFactory),
        portfolioTreeRecordFactory(_portfolioTreeRecordFactory),
        instrumentWidgetFactory(_instrumentWidgetFactory),
        userStorage(_userStorage),
        instrumentsStorage(_instrumentsStorage),
        logosStorage(_logosStorage),
        fileDialogFactory(_fileDialogFactory),
        settingsEditor(_settingsEditor)
    {
    }

    bool operator==(const DecisionMakerWidgetFactoryNewInstanceArgsMore15& another) const = default;

    IOperationsTableWidgetFactory* operationsTableWidgetFactory;
    IAccountChartWidgetFactory*    accountChartWidgetFactory;
    ILogsFilterWidgetFactory*      logsFilterWidgetFactory;
    ILogsTableWidgetFactory*       logsTableWidgetFactory;
    IPortfolioTreeWidgetFactory*   portfolioTreeWidgetFactory;
    IOperationsTableModelFactory*  operationsTableModelFactory;
    ILogsTableModelFactory*        logsTableModelFactory;
    IPortfolioTreeModelFactory*    portfolioTreeModelFactory;
    IPortfolioTreeRecordFactory*   portfolioTreeRecordFactory;
    IInstrumentWidgetFactory*      instrumentWidgetFactory;
    IUserStorage*                  userStorage;
    IInstrumentsStorage*           instrumentsStorage;
    ILogosStorage*                 logosStorage;
    IFileDialogFactory*            fileDialogFactory;
    ISettingsEditor*               settingsEditor;
};



class IDecisionMakerWidgetFactory
{
public:
    IDecisionMakerWidgetFactory()          = default;
    virtual ~IDecisionMakerWidgetFactory() = default;

    IDecisionMakerWidgetFactory(const IDecisionMakerWidgetFactory& another)            = delete;
    IDecisionMakerWidgetFactory& operator=(const IDecisionMakerWidgetFactory& another) = delete;

    virtual IDecisionMakerWidget*
    newInstance(const DecisionMakerWidgetFactoryNewInstanceArgsMore15& args, QWidget* parent) const = 0;
};
