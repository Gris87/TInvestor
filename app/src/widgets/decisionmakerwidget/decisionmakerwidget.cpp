#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <QDebug>



// TODO: Move to TableRecord
#define OPERATIONS_TIME_COLUMN      0
#define OPERATIONS_NAME_COLUMN      1
#define OPERATIONS_OPERATION_COLUMN 2
#define OPERATIONS_PRICE_COLUMN     3
#define OPERATIONS_QUANTITY_COLUMN  4
#define OPERATIONS_COST_COLUMN      5

#define LOGS_TIME_COLUMN    0
#define LOGS_MESSAGE_COLUMN 1

#define PORTFOLIO_NAME_COLUMN           0
#define PORTFOLIO_AVAILABLE_COLUMN      1
#define PORTFOLIO_PRICE_COLUMN          2
#define PORTFOLIO_AVG_PRICE_COLUMN      3
#define PORTFOLIO_COST_COLUMN           4
#define PORTFOLIO_PART_COLUMN           5
#define PORTFOLIO_INCOME_COLUMN         6
#define PORTFOLIO_INCOME_PERCENT_COLUMN 7
#define PORTFOLIO_IN_A_DAY_COLUMN       8



DecisionMakerWidget::DecisionMakerWidget(ISettingsEditor* settingsEditor, const QString& type, QWidget* parent) :
    IDecisionMakerWidget(parent),
    ui(new Ui::DecisionMakerWidget),
    mSettingsEditor(settingsEditor),
    mType(type)
{
    qDebug() << "Create DecisionMakerWidget";

    ui->setupUi(this);

    ui->tabWidget->setCurrentWidget(ui->operationsTab);
}

DecisionMakerWidget::~DecisionMakerWidget()
{
    qDebug() << "Destroy DecisionMakerWidget";

    delete ui;
}

void DecisionMakerWidget::saveWindowState()
{
    QString type = "MainWindow/" + mType;

    mSettingsEditor->setValue(type + "/splitter", ui->splitter->saveState());

    // clang-format off
    mSettingsEditor->setValue(type + "/operationsTableWidget_Time",      ui->operationsTableWidget->columnWidth(OPERATIONS_TIME_COLUMN));
    mSettingsEditor->setValue(type + "/operationsTableWidget_Name",      ui->operationsTableWidget->columnWidth(OPERATIONS_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/operationsTableWidget_Operation", ui->operationsTableWidget->columnWidth(OPERATIONS_OPERATION_COLUMN));
    mSettingsEditor->setValue(type + "/operationsTableWidget_Price",     ui->operationsTableWidget->columnWidth(OPERATIONS_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/operationsTableWidget_Quantity",  ui->operationsTableWidget->columnWidth(OPERATIONS_QUANTITY_COLUMN));
    mSettingsEditor->setValue(type + "/operationsTableWidget_Cost",      ui->operationsTableWidget->columnWidth(OPERATIONS_COST_COLUMN));
    // clang-format on

    // clang-format off
    mSettingsEditor->setValue(type + "/logsTableWidget_Time",    ui->logsTableWidget->columnWidth(LOGS_TIME_COLUMN));
    mSettingsEditor->setValue(type + "/logsTableWidget_Message", ui->logsTableWidget->columnWidth(LOGS_MESSAGE_COLUMN));
    // clang-format on

    // clang-format off
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Name",          ui->portfolioTableWidget->columnWidth(PORTFOLIO_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Available",     ui->portfolioTableWidget->columnWidth(PORTFOLIO_AVAILABLE_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Price",         ui->portfolioTableWidget->columnWidth(PORTFOLIO_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_AvgPrice",      ui->portfolioTableWidget->columnWidth(PORTFOLIO_AVG_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Cost",          ui->portfolioTableWidget->columnWidth(PORTFOLIO_COST_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Part",          ui->portfolioTableWidget->columnWidth(PORTFOLIO_PART_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Income",        ui->portfolioTableWidget->columnWidth(PORTFOLIO_INCOME_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_IncomePercent", ui->portfolioTableWidget->columnWidth(PORTFOLIO_INCOME_PERCENT_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_InADay",        ui->portfolioTableWidget->columnWidth(PORTFOLIO_IN_A_DAY_COLUMN));
    // clang-format on
}

void DecisionMakerWidget::loadWindowState()
{
    QString type = "MainWindow/" + mType;

    ui->splitter->restoreState(mSettingsEditor->value(type + "/splitter", QByteArray()).toByteArray());

    // clang-format off
    ui->operationsTableWidget->setColumnWidth(OPERATIONS_TIME_COLUMN,      mSettingsEditor->value(type + "/operationsTableWidget_Time",      42).toInt());
    ui->operationsTableWidget->setColumnWidth(OPERATIONS_NAME_COLUMN,      mSettingsEditor->value(type + "/operationsTableWidget_Name",      60).toInt());
    ui->operationsTableWidget->setColumnWidth(OPERATIONS_OPERATION_COLUMN, mSettingsEditor->value(type + "/operationsTableWidget_Operation", 63).toInt());
    ui->operationsTableWidget->setColumnWidth(OPERATIONS_PRICE_COLUMN,     mSettingsEditor->value(type + "/operationsTableWidget_Price",     34).toInt());
    ui->operationsTableWidget->setColumnWidth(OPERATIONS_QUANTITY_COLUMN,  mSettingsEditor->value(type + "/operationsTableWidget_Quantity",  73).toInt());
    ui->operationsTableWidget->setColumnWidth(OPERATIONS_COST_COLUMN,      mSettingsEditor->value(type + "/operationsTableWidget_Cost",      65).toInt());
    // clang-format on

    // clang-format off
    ui->logsTableWidget->setColumnWidth(LOGS_TIME_COLUMN,    mSettingsEditor->value(type + "/logsTableWidget_Time",    42).toInt());
    ui->logsTableWidget->setColumnWidth(LOGS_MESSAGE_COLUMN, mSettingsEditor->value(type + "/logsTableWidget_Message", 72).toInt());
    // clang-format on

    // clang-format off
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_NAME_COLUMN,           mSettingsEditor->value(type + "/portfolioTableWidget_Name",          60).toInt());
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_AVAILABLE_COLUMN,      mSettingsEditor->value(type + "/portfolioTableWidget_Available",     58).toInt());
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_PRICE_COLUMN,          mSettingsEditor->value(type + "/portfolioTableWidget_Price",         34).toInt());
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_AVG_PRICE_COLUMN,      mSettingsEditor->value(type + "/portfolioTableWidget_AvgPrice",      84).toInt());
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_COST_COLUMN,           mSettingsEditor->value(type + "/portfolioTableWidget_Cost",          65).toInt());
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_PART_COLUMN,           mSettingsEditor->value(type + "/portfolioTableWidget_Part",          34).toInt());
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_INCOME_COLUMN,         mSettingsEditor->value(type + "/portfolioTableWidget_Income",        41).toInt());
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_INCOME_PERCENT_COLUMN, mSettingsEditor->value(type + "/portfolioTableWidget_IncomePercent", 57).toInt());
    ui->portfolioTableWidget->setColumnWidth(PORTFOLIO_IN_A_DAY_COLUMN,       mSettingsEditor->value(type + "/portfolioTableWidget_InADay",        64).toInt());
    // clang-format on
}
