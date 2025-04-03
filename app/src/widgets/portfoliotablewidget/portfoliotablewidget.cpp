#include "src/widgets/portfoliotablewidget/portfoliotablewidget.h"
#include "ui_portfoliotablewidget.h"

#include <QDebug>



// TODO: Move to PortfolioTableRecord
#define PORTFOLIO_NAME_COLUMN           0
#define PORTFOLIO_AVAILABLE_COLUMN      1
#define PORTFOLIO_PRICE_COLUMN          2
#define PORTFOLIO_AVG_PRICE_COLUMN      3
#define PORTFOLIO_COST_COLUMN           4
#define PORTFOLIO_PART_COLUMN           5
#define PORTFOLIO_INCOME_COLUMN         6
#define PORTFOLIO_INCOME_PERCENT_COLUMN 7
#define PORTFOLIO_IN_A_DAY_COLUMN       8



PortfolioTableWidget::PortfolioTableWidget(ISettingsEditor* settingsEditor, QWidget* parent) :
    IPortfolioTableWidget(parent),
    ui(new Ui::PortfolioTableWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create PortfolioTableWidget";

    ui->setupUi(this);
}

PortfolioTableWidget::~PortfolioTableWidget()
{
    qDebug() << "Destroy PortfolioTableWidget";

    delete ui;
}

void PortfolioTableWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Name",          ui->tableWidget->columnWidth(PORTFOLIO_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Available",     ui->tableWidget->columnWidth(PORTFOLIO_AVAILABLE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Price",         ui->tableWidget->columnWidth(PORTFOLIO_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_AvgPrice",      ui->tableWidget->columnWidth(PORTFOLIO_AVG_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Cost",          ui->tableWidget->columnWidth(PORTFOLIO_COST_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Part",          ui->tableWidget->columnWidth(PORTFOLIO_PART_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Income",        ui->tableWidget->columnWidth(PORTFOLIO_INCOME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_IncomePercent", ui->tableWidget->columnWidth(PORTFOLIO_INCOME_PERCENT_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_InADay",        ui->tableWidget->columnWidth(PORTFOLIO_IN_A_DAY_COLUMN));
    // clang-format on
}

void PortfolioTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(PORTFOLIO_NAME_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Name",          60).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_AVAILABLE_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Available",     58).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_PRICE_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Price",         34).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_AVG_PRICE_COLUMN,      mSettingsEditor->value(type + "/columnWidth_AvgPrice",      84).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_COST_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Cost",          65).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_PART_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Part",          34).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_INCOME_COLUMN,         mSettingsEditor->value(type + "/columnWidth_Income",        41).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_INCOME_PERCENT_COLUMN, mSettingsEditor->value(type + "/columnWidth_IncomePercent", 57).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_IN_A_DAY_COLUMN,       mSettingsEditor->value(type + "/columnWidth_InADay",        64).toInt());
    // clang-format on
}
