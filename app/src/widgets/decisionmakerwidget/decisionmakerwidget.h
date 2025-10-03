#pragma once



#include "src/widgets/decisionmakerwidget/idecisionmakerwidget.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/accountchartwidget/iaccountchartwidgetfactory.h"
#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory.h"
#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"
#include "src/widgets/operationstablewidget/ioperationstablewidgetfactory.h"
#include "src/widgets/portfoliotreewidget/iportfoliotreewidgetfactory.h"



namespace Ui
{
class DecisionMakerWidget;
}



class DecisionMakerWidget : public IDecisionMakerWidget
{
    Q_OBJECT

public:
    explicit DecisionMakerWidget(
        IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
        IAccountChartWidgetFactory*        accountChartWidgetFactory,
        ILogsFilterWidgetFactory*          logsFilterWidgetFactory,
        ILogsTableWidgetFactory*           logsTableWidgetFactory,
        IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
        IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory,
        IBuyDecision5ConfigWidgetFactory*  buyDecision5ConfigWidgetFactory,
        IBuyDecision6ConfigWidgetFactory*  buyDecision6ConfigWidgetFactory,
        IBuyDecision7ConfigWidgetFactory*  buyDecision7ConfigWidgetFactory,
        IBuyDecision8ConfigWidgetFactory*  buyDecision8ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
        ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory,
        ISellDecision5ConfigWidgetFactory* sellDecision5ConfigWidgetFactory,
        IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
        IOperationsTableModelFactory*      operationsTableModelFactory,
        ILogsTableModelFactory*            logsTableModelFactory,
        IPortfolioTreeModelFactory*        portfolioTreeModelFactory,
        IFileDialogFactory*                fileDialogFactory,
        IConfig*                           config,
        IConfig*                           configForSimulation,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent = nullptr
    );
    ~DecisionMakerWidget() override;

    DecisionMakerWidget(const DecisionMakerWidget& another)            = delete;
    DecisionMakerWidget& operator=(const DecisionMakerWidget& another) = delete;

    void setAccountName(const QString& name) override;

    void showSpinners() override;
    void bestConfigFound() override;
    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;
    void logsRead(const QList<LogEntry>& entries) override;
    void logAdded(const LogEntry& entry) override;
    void portfolioChanged(const Portfolio& portfolio) override;
    void lastPriceChanged(const QString& instrumentId, float price) override;
    void updateLastPrices() override;
    void refreshOperationsBackground() override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

    Ui::DecisionMakerWidget* ui;

private:
    IOperationsTableWidget*     mOperationsTableWidget;
    IAccountChartWidget*        mAccountChartWidget;
    ILogsFilterWidget*          mLogsFilterWidget;
    ILogsTableWidget*           mLogsTableWidget;
    IDecisionMakerConfigWidget* mBestConfigWidget;
    IPortfolioTreeWidget*       mPortfolioTreeWidget;
    IConfig*                    mConfig;
    IConfig*                    mConfigForSimulation;
    ISettingsEditor*            mSettingsEditor;
    QGraphicsOpacityEffect      mCopiedOpacityEffect;
    QPropertyAnimation          mCopiedOpacityAnimation;

public slots:
    void logFilterChanged(const LogFilter& filter);

private slots:
    void on_yieldButton_clicked();
    void on_monthlyYieldButton_clicked();
    void on_dailyYieldButton_clicked();
    void on_remainedMoneyButton_clicked();
    void on_totalMoneyButton_clicked();
    void on_copyToSimulatorConfigButton_clicked();
    void on_copyToAutoPilotConfigButton_clicked();
    void on_copyToBothConfigsButton_clicked();
    void on_timeRangeComboBox_currentIndexChanged(int index);
};
