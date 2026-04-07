#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <QDebug>



const QColor GREY_COLOR = QColor("#AFC2D7"); // clazy:exclude=non-pod-global-static

constexpr qint64 MS_IN_SECOND      = 1000LL;
constexpr qint64 HIDE_COPIED_DELAY = 2 * MS_IN_SECOND; // 2 seconds

constexpr int BEST_CONFIG_TAB_ID = 3;



DecisionMakerWidget::DecisionMakerWidget(
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
    IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
    IOperationsTableModelFactory*      operationsTableModelFactory,
    ILogsTableModelFactory*            logsTableModelFactory,
    IPortfolioTreeModelFactory*        portfolioTreeModelFactory,
    IFileDialogFactory*                fileDialogFactory,
    IMessageBoxUtils*                  messageBoxUtils,
    IConfig*                           config,
    IConfig*                           configForSimulation,
    ISettingsEditor*                   settingsEditor,
    bool                               autoPilot,
    QWidget*                           parent
) :
    IDecisionMakerWidget(parent),
    ui(new Ui::DecisionMakerWidget),
    mConfig(config),
    mConfigForSimulation(configForSimulation),
    mSettingsEditor(settingsEditor),
    mShowMoney(true)
{
    qDebug() << "Create DecisionMakerWidget";

    ui->setupUi(this);

    ui->operationsWaitingSpinnerWidget->setColor(GREY_COLOR);
    ui->operationsWaitingSpinnerWidget->setTextColor(GREY_COLOR);
    ui->portfolioWaitingSpinnerWidget->setColor(GREY_COLOR);
    ui->portfolioWaitingSpinnerWidget->setTextColor(GREY_COLOR);
    ui->operationsWaitingSpinnerWidget->setText(tr("Loading"));
    ui->portfolioWaitingSpinnerWidget->setText(tr("Loading"));

    ui->splitter->setSizes(QList<int>() << 600 << 400); // NOLINT(readability-magic-numbers)

    mCopiedOpacityEffect.setOpacity(0.0f);
    mCopiedOpacityAnimation.setTargetObject(&mCopiedOpacityEffect);
    mCopiedOpacityAnimation.setPropertyName("opacity");
    mCopiedOpacityAnimation.setDuration(HIDE_COPIED_DELAY);
    mCopiedOpacityAnimation.setStartValue(1.0f);
    mCopiedOpacityAnimation.setEndValue(0.0f);
    ui->copiedLabel->setGraphicsEffect(&mCopiedOpacityEffect);

    mOperationsTableWidget =
        operationsTableWidgetFactory->newInstance(operationsTableModelFactory, fileDialogFactory, config, mSettingsEditor, this);
    mAccountChartWidget = accountChartWidgetFactory->newInstance(fileDialogFactory, mSettingsEditor, this);
    mLogsFilterWidget   = logsFilterWidgetFactory->newInstance(this);
    mLogsTableWidget    = logsTableWidgetFactory->newInstance(logsTableModelFactory, fileDialogFactory, mSettingsEditor, this);
    mBestConfigWidget   = decisionMakerConfigWidgetFactory->newInstance(
        DecisionMakerConfigWidgetFactoryNewInstanceArgsMore15(
            mConfigForSimulation->getSimulatorConfig(),
            buyDecision1ConfigWidgetFactory,
            buyDecision2ConfigWidgetFactory,
            buyDecision3ConfigWidgetFactory,
            buyDecision4ConfigWidgetFactory,
            buyDecision5ConfigWidgetFactory,
            buyDecision6ConfigWidgetFactory,
            buyDecision7ConfigWidgetFactory,
            buyDecision8ConfigWidgetFactory,
            sellDecision1ConfigWidgetFactory,
            sellDecision2ConfigWidgetFactory,
            sellDecision3ConfigWidgetFactory,
            sellDecision4ConfigWidgetFactory
        ),
        ui->bestConfigTab
    );
    mPortfolioTreeWidget = portfolioTreeWidgetFactory->newInstance(
        portfolioTreeModelFactory, fileDialogFactory, messageBoxUtils, mSettingsEditor, autoPilot, this
    );

    mLogsTableWidget->setFilter(mLogsFilterWidget->getFilter());
    mBestConfigWidget->makeReadOnly();

    ui->layoutForOperationsTableWidget->addWidget(mOperationsTableWidget);
    ui->layoutForAccountChartWidget->addWidget(mAccountChartWidget);
    ui->layoutForLogsFilterWidget->addWidget(mLogsFilterWidget);
    ui->layoutForLogsTableWidget->addWidget(mLogsTableWidget);
    ui->layoutForBestConfigWidget->addWidget(mBestConfigWidget);
    ui->layoutForPortfolioTreeWidget->addWidget(mPortfolioTreeWidget);

    ui->tabWidget->removeTab(BEST_CONFIG_TAB_ID);
    ui->tabWidget->setCurrentWidget(ui->operationsTab);

    // clang-format off
    connect(mLogsFilterWidget,    SIGNAL(filterChanged(const LogFilter&)),                this, SLOT(logFilterChanged(const LogFilter&)));
    connect(mPortfolioTreeWidget, SIGNAL(tradeInstruments(const InstrumentsForTrading&)), this, SLOT(portfolioTreeWidgetManualSell(const InstrumentsForTrading&)));
    // clang-format on
}

DecisionMakerWidget::~DecisionMakerWidget()
{
    qDebug() << "Destroy DecisionMakerWidget";

    delete ui;
}

void DecisionMakerWidget::setAccountName(const QString& name)
{
    mPortfolioTreeWidget->setAccountName(name);
}

bool DecisionMakerWidget::isShowMoney() const
{
    return mShowMoney;
}

void DecisionMakerWidget::setShowMoney(bool value)
{
    mShowMoney = value;

    mOperationsTableWidget->setShowMoney(value);
    mPortfolioTreeWidget->setShowMoney(value);
}

void DecisionMakerWidget::showSpinners()
{
    ui->operationsWaitingSpinnerWidget->start();
    ui->portfolioWaitingSpinnerWidget->start();

    ui->operationsWaitingStackedWidget->setCurrentWidget(ui->operationsWaitingPage);
    ui->portfolioWaitingStackedWidget->setCurrentWidget(ui->portfolioWaitingPage);

    if (ui->tabWidget->count() > BEST_CONFIG_TAB_ID)
    {
        ui->tabWidget->removeTab(BEST_CONFIG_TAB_ID);
    }
}

void DecisionMakerWidget::bestConfigFound()
{
    if (ui->tabWidget->count() <= BEST_CONFIG_TAB_ID)
    {
        ui->tabWidget->insertTab(BEST_CONFIG_TAB_ID, ui->bestConfigTab, tr("Best config"));
    }

    mBestConfigWidget->updateUiFromConfig();
}

void DecisionMakerWidget::operationsRead(const QList<Operation>& operations)
{
    mOperationsTableWidget->operationsRead(operations);
    mAccountChartWidget->operationsRead(operations);

    ui->operationsWaitingSpinnerWidget->stop();
    ui->operationsWaitingStackedWidget->setCurrentWidget(ui->operationsWorkingPage);
}

void DecisionMakerWidget::operationsAdded(const QList<Operation>& operations)
{
    mOperationsTableWidget->operationsAdded(operations);
    mAccountChartWidget->operationsAdded(operations);
}

void DecisionMakerWidget::logsRead(const QList<LogEntry>& entries)
{
    mLogsTableWidget->logsRead(entries);
}

void DecisionMakerWidget::logAdded(const LogEntry& entry)
{
    mLogsTableWidget->logAdded(entry);
}

void DecisionMakerWidget::portfolioChanged(const Portfolio& portfolio)
{
    mPortfolioTreeWidget->portfolioChanged(portfolio);

    ui->portfolioWaitingSpinnerWidget->stop();
    ui->portfolioWaitingStackedWidget->setCurrentWidget(ui->portfolioWorkingPage);
}

void DecisionMakerWidget::lastPriceChanged(const QString& instrumentId, float price)
{
    mPortfolioTreeWidget->lastPriceChanged(instrumentId, price);
}

void DecisionMakerWidget::updateLastPrices()
{
    mPortfolioTreeWidget->updateLastPrices();
}

void DecisionMakerWidget::refreshOperationsBackground()
{
    mOperationsTableWidget->refreshBackground();
}

void DecisionMakerWidget::logFilterChanged(const LogFilter& filter)
{
    mLogsTableWidget->setFilter(filter);
}

void DecisionMakerWidget::portfolioTreeWidgetManualSell(const InstrumentsForTrading& instruments)
{
    emit tradeInstruments(instruments);
}

void DecisionMakerWidget::on_yieldButton_clicked()
{
    mAccountChartWidget->switchChart(CHART_TYPE_YIELD);

    ui->yieldButton->setChecked(true);
    ui->monthlyYieldButton->setChecked(false);
    ui->dailyYieldButton->setChecked(false);
    ui->totalMoneyButton->setChecked(false);

    ui->timeRangeComboBox->setEnabled(true);
}

void DecisionMakerWidget::on_monthlyYieldButton_clicked()
{
    mAccountChartWidget->switchChart(CHART_TYPE_MONTHLY_YIELD);

    ui->yieldButton->setChecked(false);
    ui->monthlyYieldButton->setChecked(true);
    ui->dailyYieldButton->setChecked(false);
    ui->totalMoneyButton->setChecked(false);

    ui->timeRangeComboBox->setEnabled(false);
}

void DecisionMakerWidget::on_dailyYieldButton_clicked()
{
    mAccountChartWidget->switchChart(CHART_TYPE_DAILY_YIELD);

    ui->yieldButton->setChecked(false);
    ui->monthlyYieldButton->setChecked(false);
    ui->dailyYieldButton->setChecked(true);
    ui->totalMoneyButton->setChecked(false);

    ui->timeRangeComboBox->setEnabled(false);
}

void DecisionMakerWidget::on_totalMoneyButton_clicked()
{
    mAccountChartWidget->switchChart(CHART_TYPE_TOTAL_MONEY);

    ui->yieldButton->setChecked(false);
    ui->monthlyYieldButton->setChecked(false);
    ui->dailyYieldButton->setChecked(false);
    ui->totalMoneyButton->setChecked(true);

    ui->timeRangeComboBox->setEnabled(true);
}

void DecisionMakerWidget::on_timeRangeComboBox_currentIndexChanged(int index)
{
    mAccountChartWidget->setTimeRange(static_cast<TimeRange>(index));
}

void DecisionMakerWidget::on_copyToSimulatorConfigButton_clicked()
{
    mConfig->setSimulatorConfigCommon(false);
    mConfig->setAutoPilotConfigCommon(false);

    mConfig->getSimulatorConfig()->assign(mConfigForSimulation->getSimulatorConfig());
    mConfig->save(mSettingsEditor);

    mCopiedOpacityAnimation.stop();
    mCopiedOpacityAnimation.start();
}

void DecisionMakerWidget::on_copyToAutoPilotConfigButton_clicked()
{
    mConfig->setSimulatorConfigCommon(false);
    mConfig->setAutoPilotConfigCommon(false);

    mConfig->getAutoPilotConfig()->assign(mConfigForSimulation->getSimulatorConfig());
    mConfig->save(mSettingsEditor);

    mCopiedOpacityAnimation.stop();
    mCopiedOpacityAnimation.start();
}

void DecisionMakerWidget::on_copyToBothConfigsButton_clicked()
{
    mConfig->setSimulatorConfigCommon(true);
    mConfig->setAutoPilotConfigCommon(false);

    mConfig->getSimulatorConfig()->assign(mConfigForSimulation->getSimulatorConfig());
    mConfig->getAutoPilotConfig()->assign(mConfigForSimulation->getSimulatorConfig());
    mConfig->save(mSettingsEditor);

    mCopiedOpacityAnimation.stop();
    mCopiedOpacityAnimation.start();
}

void DecisionMakerWidget::saveWindowState(const QString& type)
{
    mSettingsEditor->setValue(type + "/splitter", ui->splitter->saveState());

    mOperationsTableWidget->saveWindowState(type + "/OperationsTableWidget");
    mLogsTableWidget->saveWindowState(type + "/LogsTableWidget");
    mPortfolioTreeWidget->saveWindowState(type + "/PortfolioTreeWidget");
}

void DecisionMakerWidget::loadWindowState(const QString& type)
{
    ui->splitter->restoreState(mSettingsEditor->value(type + "/splitter", QByteArray()).toByteArray());

    mOperationsTableWidget->loadWindowState(type + "/OperationsTableWidget");
    mLogsTableWidget->loadWindowState(type + "/LogsTableWidget");
    mPortfolioTreeWidget->loadWindowState(type + "/PortfolioTreeWidget");
}
