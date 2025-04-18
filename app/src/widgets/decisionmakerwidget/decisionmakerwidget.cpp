#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <QDebug>



DecisionMakerWidget::DecisionMakerWidget(
    IOperationsTableWidgetFactory* operationsTableWidgetFactory,
    IAccountChartWidgetFactory*    accountChartWidgetFactory,
    ILogsTableWidgetFactory*       logsTableWidgetFactory,
    IPortfolioTableWidgetFactory*  portfolioTableWidgetFactory,
    ISettingsEditor*               settingsEditor,
    QWidget*                       parent
) :
    IDecisionMakerWidget(parent),
    ui(new Ui::DecisionMakerWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create DecisionMakerWidget";

    ui->setupUi(this);

    mOperationsTableWidget = operationsTableWidgetFactory->newInstance(mSettingsEditor, this);
    mAccountChartWidget    = accountChartWidgetFactory->newInstance(this);
    mLogsTableWidget       = logsTableWidgetFactory->newInstance(mSettingsEditor, this);
    mPortfolioTableWidget  = portfolioTableWidgetFactory->newInstance(mSettingsEditor, this);

    ui->layoutForOperationsTableWidget->addWidget(mOperationsTableWidget);
    ui->layoutForAccountChartWidget->addWidget(mAccountChartWidget);
    ui->layoutForLogsTableWidget->addWidget(mLogsTableWidget);
    ui->layoutForPortfolioTableWidget->addWidget(mPortfolioTableWidget);

    ui->tabWidget->setCurrentWidget(ui->operationsTab);
}

DecisionMakerWidget::~DecisionMakerWidget()
{
    qDebug() << "Destroy DecisionMakerWidget";

    delete ui;
}

void DecisionMakerWidget::saveWindowState(const QString& type)
{
    mSettingsEditor->setValue(type + "/splitter", ui->splitter->saveState());

    mOperationsTableWidget->saveWindowState(type + "/OperationsTableWidget");
    mLogsTableWidget->saveWindowState(type + "/LogsTableWidget");
    mPortfolioTableWidget->saveWindowState(type + "/PortfolioTableWidget");
}

void DecisionMakerWidget::loadWindowState(const QString& type)
{
    ui->splitter->restoreState(mSettingsEditor->value(type + "/splitter", QByteArray()).toByteArray());

    mOperationsTableWidget->loadWindowState(type + "/OperationsTableWidget");
    mLogsTableWidget->loadWindowState(type + "/LogsTableWidget");
    mPortfolioTableWidget->loadWindowState(type + "/PortfolioTableWidget");
}
