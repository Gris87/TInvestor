#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <QDebug>



const QColor GREY_COLOR = QColor("#AFC2D7"); // clazy:exclude=non-pod-global-static



DecisionMakerWidget::DecisionMakerWidget(
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
) :
    IDecisionMakerWidget(parent),
    ui(new Ui::DecisionMakerWidget),
    mSettingsEditor(settingsEditor)
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

    mOperationsTableWidget =
        operationsTableWidgetFactory->newInstance(operationsTableModelFactory, fileDialogFactory, mSettingsEditor, this);
    mAccountChartWidget = accountChartWidgetFactory->newInstance(fileDialogFactory, mSettingsEditor, this);
    mLogsFilterWidget   = logsFilterWidgetFactory->newInstance(this);
    mLogsTableWidget    = logsTableWidgetFactory->newInstance(logsTableModelFactory, fileDialogFactory, mSettingsEditor, this);
    mPortfolioTreeWidget =
        portfolioTreeWidgetFactory->newInstance(portfolioTreeModelFactory, fileDialogFactory, mSettingsEditor, this);

    mLogsTableWidget->setFilter(mLogsFilterWidget->getFilter());

    ui->layoutForOperationsTableWidget->addWidget(mOperationsTableWidget);
    ui->layoutForAccountChartWidget->addWidget(mAccountChartWidget);
    ui->layoutForLogsFilterWidget->addWidget(mLogsFilterWidget);
    ui->layoutForLogsTableWidget->addWidget(mLogsTableWidget);
    ui->layoutForPortfolioTreeWidget->addWidget(mPortfolioTreeWidget);

    ui->tabWidget->setCurrentWidget(ui->operationsTab);

    connect(mLogsFilterWidget, SIGNAL(filterChanged(const LogFilter&)), this, SLOT(logFilterChanged(const LogFilter&)));
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

void DecisionMakerWidget::showSpinners()
{
    ui->operationsWaitingSpinnerWidget->start();
    ui->portfolioWaitingSpinnerWidget->start();

    ui->operationsWaitingStackedWidget->setCurrentWidget(ui->operationsWaitingPage);
    ui->portfolioWaitingStackedWidget->setCurrentWidget(ui->portfolioWaitingPage);
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

void DecisionMakerWidget::logFilterChanged(const LogFilter& filter)
{
    mLogsTableWidget->setFilter(filter);
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
