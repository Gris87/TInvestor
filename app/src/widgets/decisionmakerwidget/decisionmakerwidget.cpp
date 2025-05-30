#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <QDebug>



DecisionMakerWidget::DecisionMakerWidget(
    IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
    IAccountChartWidgetFactory*        accountChartWidgetFactory,
    ILogsTableWidgetFactory*           logsTableWidgetFactory,
    IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
    IOperationsTableRecordFactory*     operationsTableRecordFactory,
    IPortfolioTreeRecordFactory*       portfolioTreeRecordFactory,
    IInstrumentWidgetFactory*          instrumentWidgetFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
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

    ui->splitter->setSizes(QList<int>() << 400 << 100); // NOLINT(readability-magic-numbers)

    mOperationsTableWidget = operationsTableWidgetFactory->newInstance(
        operationsTableRecordFactory,
        instrumentTableItemWidgetFactory,
        userStorage,
        instrumentsStorage,
        fileDialogFactory,
        mSettingsEditor,
        this
    );
    mAccountChartWidget  = accountChartWidgetFactory->newInstance(fileDialogFactory, mSettingsEditor, this);
    mLogsTableWidget     = logsTableWidgetFactory->newInstance(mSettingsEditor, this);
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

void DecisionMakerWidget::portfolioChanged(const Portfolio& portfolio)
{
    mPortfolioTreeWidget->portfolioChanged(portfolio);
}

void DecisionMakerWidget::lastPriceChanged(const QString& instrumentId, float price)
{
    mPortfolioTreeWidget->lastPriceChanged(instrumentId, price);
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
