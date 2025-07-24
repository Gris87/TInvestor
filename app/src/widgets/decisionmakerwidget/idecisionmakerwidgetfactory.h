#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"
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
        IOperationsTableWidgetFactory*     _operationsTableWidgetFactory,
        IAccountChartWidgetFactory*        _accountChartWidgetFactory,
        ILogsFilterWidgetFactory*          _logsFilterWidgetFactory,
        ILogsTableWidgetFactory*           _logsTableWidgetFactory,
        IDecisionMakerConfigWidgetFactory* _decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  _buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  _buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  _buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* _sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* _sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* _sellDecision3ConfigWidgetFactory,
        IPortfolioTreeWidgetFactory*       _portfolioTreeWidgetFactory,
        IOperationsTableModelFactory*      _operationsTableModelFactory,
        ILogsTableModelFactory*            _logsTableModelFactory,
        IPortfolioTreeModelFactory*        _portfolioTreeModelFactory,
        IFileDialogFactory*                _fileDialogFactory,
        IConfig*                           _config,
        IConfig*                           _configForSimulation,
        ISettingsEditor*                   _settingsEditor
    ) :
        operationsTableWidgetFactory(_operationsTableWidgetFactory),
        accountChartWidgetFactory(_accountChartWidgetFactory),
        logsFilterWidgetFactory(_logsFilterWidgetFactory),
        logsTableWidgetFactory(_logsTableWidgetFactory),
        decisionMakerConfigWidgetFactory(_decisionMakerConfigWidgetFactory),
        buyDecision1ConfigWidgetFactory(_buyDecision1ConfigWidgetFactory),
        buyDecision2ConfigWidgetFactory(_buyDecision2ConfigWidgetFactory),
        buyDecision3ConfigWidgetFactory(_buyDecision3ConfigWidgetFactory),
        sellDecision1ConfigWidgetFactory(_sellDecision1ConfigWidgetFactory),
        sellDecision2ConfigWidgetFactory(_sellDecision2ConfigWidgetFactory),
        sellDecision3ConfigWidgetFactory(_sellDecision3ConfigWidgetFactory),
        portfolioTreeWidgetFactory(_portfolioTreeWidgetFactory),
        operationsTableModelFactory(_operationsTableModelFactory),
        logsTableModelFactory(_logsTableModelFactory),
        portfolioTreeModelFactory(_portfolioTreeModelFactory),
        fileDialogFactory(_fileDialogFactory),
        config(_config),
        configForSimulation(_configForSimulation),
        settingsEditor(_settingsEditor)
    {
    }

    bool operator==(const DecisionMakerWidgetFactoryNewInstanceArgsMore15& another) const = default;

    IOperationsTableWidgetFactory*     operationsTableWidgetFactory;
    IAccountChartWidgetFactory*        accountChartWidgetFactory;
    ILogsFilterWidgetFactory*          logsFilterWidgetFactory;
    ILogsTableWidgetFactory*           logsTableWidgetFactory;
    IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory;
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory;
    IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory;
    IOperationsTableModelFactory*      operationsTableModelFactory;
    ILogsTableModelFactory*            logsTableModelFactory;
    IPortfolioTreeModelFactory*        portfolioTreeModelFactory;
    IFileDialogFactory*                fileDialogFactory;
    IConfig*                           config;
    IConfig*                           configForSimulation;
    ISettingsEditor*                   settingsEditor;
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
