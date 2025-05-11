#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"
#include "src/widgets/portfoliotablewidget/iportfoliotablewidgetfactory.h"



namespace Ui
{
class DecisionMakerWidget;
}



class DecisionMakerWidget : public IDecisionMakerWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerWidget(
        IOperationsTableWidgetFactory* operationsTableWidgetFactory,
        IAccountChartWidgetFactory*    accountChartWidgetFactory,
        ILogsTableWidgetFactory*       logsTableWidgetFactory,
        IPortfolioTableWidgetFactory*  portfolioTableWidgetFactory,
        ISettingsEditor*               settingsEditor,
        QWidget*                       parent = nullptr
    );
    ~DecisionMakerWidget() override;

    DecisionMakerWidget(const DecisionMakerWidget& another)            = delete;
    DecisionMakerWidget& operator=(const DecisionMakerWidget& another) = delete;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

    Ui::DecisionMakerWidget* ui;

private:
    IOperationsTableWidget* mOperationsTableWidget;
    IAccountChartWidget*    mAccountChartWidget;
    ILogsTableWidget*       mLogsTableWidget;
    IPortfolioTableWidget*  mPortfolioTableWidget;
    ISettingsEditor*        mSettingsEditor;
};
