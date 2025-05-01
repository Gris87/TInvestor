#include "src/widgets/portfoliotablewidget/portfoliotablewidget.h"
#include "ui_portfoliotablewidget.h"

#include <QDebug>



// TODO: Move to PortfolioTableRecord
enum PortfolioTableColumns : qint8
{
    PORTFOLIO_NAME_COLUMN,
    PORTFOLIO_AVAILABLE_COLUMN,
    PORTFOLIO_PRICE_COLUMN,
    PORTFOLIO_AVG_PRICE_COLUMN,
    PORTFOLIO_COST_COLUMN,
    PORTFOLIO_PART_COLUMN,
    PORTFOLIO_INCOME_COLUMN,
    PORTFOLIO_INCOME_PERCENT_COLUMN,
    PORTFOLIO_IN_A_DAY_COLUMN,
    COLUMN_COUNT,
};



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[COLUMN_COUNT] = {82, 80, 56, 106, 87, 56, 63, 79, 86};
#else
const int COLUMN_WIDTHS[COLUMN_COUNT] = {89, 87, 59, 114, 95, 59, 66, 83, 93};
#endif



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

// NOLINTBEGIN(readability-magic-numbers)
void PortfolioTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(PORTFOLIO_NAME_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Name",          COLUMN_WIDTHS[PORTFOLIO_NAME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_AVAILABLE_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Available",     COLUMN_WIDTHS[PORTFOLIO_AVAILABLE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_PRICE_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Price",         COLUMN_WIDTHS[PORTFOLIO_PRICE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_AVG_PRICE_COLUMN,      mSettingsEditor->value(type + "/columnWidth_AvgPrice",      COLUMN_WIDTHS[PORTFOLIO_AVG_PRICE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_COST_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Cost",          COLUMN_WIDTHS[PORTFOLIO_COST_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_PART_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Part",          COLUMN_WIDTHS[PORTFOLIO_PART_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_INCOME_COLUMN,         mSettingsEditor->value(type + "/columnWidth_Income",        COLUMN_WIDTHS[PORTFOLIO_INCOME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_INCOME_PERCENT_COLUMN, mSettingsEditor->value(type + "/columnWidth_IncomePercent", COLUMN_WIDTHS[PORTFOLIO_INCOME_PERCENT_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PORTFOLIO_IN_A_DAY_COLUMN,       mSettingsEditor->value(type + "/columnWidth_InADay",        COLUMN_WIDTHS[PORTFOLIO_IN_A_DAY_COLUMN]).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
