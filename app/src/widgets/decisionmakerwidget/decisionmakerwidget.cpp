#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <QDebug>



DecisionMakerWidget::DecisionMakerWidget(
    IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
    IAccountChartWidgetFactory*        accountChartWidgetFactory,
    ILogsTableWidgetFactory*           logsTableWidgetFactory,
    IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
    IOperationsTableRecordFactory*     operationsTableRecordFactory,
    ILogsTableRecordFactory*           logsTableRecordFactory,
    IPortfolioTreeRecordFactory*       portfolioTreeRecordFactory,
    IInstrumentWidgetFactory*          instrumentWidgetFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    IFileDialogFactory*                fileDialogFactory,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) :
    IDecisionMakerWidget(parent),
    ui(new Ui::DecisionMakerWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create DecisionMakerWidget";

    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << 600 << 400); // NOLINT(readability-magic-numbers)

    mOperationsTableWidget = operationsTableWidgetFactory->newInstance(
        operationsTableRecordFactory,
        instrumentTableItemWidgetFactory,
        userStorage,
        instrumentsStorage,
        fileDialogFactory,
        mSettingsEditor,
        this
    );
    mAccountChartWidget = accountChartWidgetFactory->newInstance(fileDialogFactory, mSettingsEditor, this);
    mLogsTableWidget    = logsTableWidgetFactory->newInstance(
        logsTableRecordFactory, logLevelTableItemWidgetFactory, fileDialogFactory, mSettingsEditor, this
    );
    mPortfolioTreeWidget = portfolioTreeWidgetFactory->newInstance(
        portfolioTreeRecordFactory,
        instrumentWidgetFactory,
        userStorage,
        instrumentsStorage,
        fileDialogFactory,
        mSettingsEditor,
        this
    );

    ui->layoutForOperationsTableWidget->addWidget(mOperationsTableWidget);
    ui->layoutForAccountChartWidget->addWidget(mAccountChartWidget);
    ui->layoutForLogsTableWidget->addWidget(mLogsTableWidget);
    ui->layoutForPortfolioTreeWidget->addWidget(mPortfolioTreeWidget);

    ui->tabWidget->setCurrentWidget(ui->operationsTab);
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

void DecisionMakerWidget::operationsRead(const QList<Operation>& operations)
{
    mOperationsTableWidget->operationsRead(operations);
    mAccountChartWidget->operationsRead(operations);
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
}

void DecisionMakerWidget::lastPriceChanged(const QString& instrumentId, float price)
{
    mPortfolioTreeWidget->lastPriceChanged(instrumentId, price);
}

void DecisionMakerWidget::updateLastPrices()
{
    mPortfolioTreeWidget->updateLastPrices();
}

void DecisionMakerWidget::on_yieldButton_clicked()
{
    mAccountChartWidget->switchChart(CHART_TYPE_YIELD);

    ui->yieldButton->setChecked(true);
    ui->monthlyYieldButton->setChecked(false);
    ui->remainedMoneyButton->setChecked(false);
    ui->totalMoneyButton->setChecked(false);
}

void DecisionMakerWidget::on_monthlyYieldButton_clicked()
{
    mAccountChartWidget->switchChart(CHART_TYPE_MONTHLY_YIELD);

    ui->yieldButton->setChecked(false);
    ui->monthlyYieldButton->setChecked(true);
    ui->remainedMoneyButton->setChecked(false);
    ui->totalMoneyButton->setChecked(false);
}

void DecisionMakerWidget::on_remainedMoneyButton_clicked()
{
    mAccountChartWidget->switchChart(CHART_TYPE_REMAINED_MONEY);

    ui->yieldButton->setChecked(false);
    ui->monthlyYieldButton->setChecked(false);
    ui->remainedMoneyButton->setChecked(true);
    ui->totalMoneyButton->setChecked(false);
}

void DecisionMakerWidget::on_totalMoneyButton_clicked()
{
    mAccountChartWidget->switchChart(CHART_TYPE_TOTAL_MONEY);

    ui->yieldButton->setChecked(false);
    ui->monthlyYieldButton->setChecked(false);
    ui->remainedMoneyButton->setChecked(false);
    ui->totalMoneyButton->setChecked(true);
}

void DecisionMakerWidget::on_verboseButton_clicked()
{
    mLogsTableWidget->setFilter(LOG_LEVEL_VERBOSE);

    ui->verboseButton->setChecked(true);
    ui->debugButton->setChecked(false);
    ui->infoButton->setChecked(false);
    ui->warningButton->setChecked(false);
    ui->errorButton->setChecked(false);
}

void DecisionMakerWidget::on_debugButton_clicked()
{
    mLogsTableWidget->setFilter(LOG_LEVEL_DEBUG);

    ui->verboseButton->setChecked(false);
    ui->debugButton->setChecked(true);
    ui->infoButton->setChecked(false);
    ui->warningButton->setChecked(false);
    ui->errorButton->setChecked(false);
}

void DecisionMakerWidget::on_infoButton_clicked()
{
    mLogsTableWidget->setFilter(LOG_LEVEL_INFO);

    ui->verboseButton->setChecked(false);
    ui->debugButton->setChecked(false);
    ui->infoButton->setChecked(true);
    ui->warningButton->setChecked(false);
    ui->errorButton->setChecked(false);
}

void DecisionMakerWidget::on_warningButton_clicked()
{
    mLogsTableWidget->setFilter(LOG_LEVEL_WARNING);

    ui->verboseButton->setChecked(false);
    ui->debugButton->setChecked(false);
    ui->infoButton->setChecked(false);
    ui->warningButton->setChecked(true);
    ui->errorButton->setChecked(false);
}

void DecisionMakerWidget::on_errorButton_clicked()
{
    mLogsTableWidget->setFilter(LOG_LEVEL_ERROR);

    ui->verboseButton->setChecked(false);
    ui->debugButton->setChecked(false);
    ui->infoButton->setChecked(false);
    ui->warningButton->setChecked(false);
    ui->errorButton->setChecked(true);
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
