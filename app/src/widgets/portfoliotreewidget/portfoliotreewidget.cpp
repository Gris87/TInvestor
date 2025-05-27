#include "src/widgets/portfoliotreewidget/portfoliotreewidget.h"
#include "ui_portfoliotreewidget.h"

#include <QDebug>



// TODO: Move to PortfolioTreeRecord
enum PortfolioTreeColumns : qint8
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



PortfolioTreeWidget::PortfolioTreeWidget(ISettingsEditor* settingsEditor, QWidget* parent) :
    IPortfolioTreeWidget(parent),
    ui(new Ui::PortfolioTreeWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create PortfolioTreeWidget";

    ui->setupUi(this);
}

PortfolioTreeWidget::~PortfolioTreeWidget()
{
    qDebug() << "Destroy PortfolioTreeWidget";

    delete ui;
}

void PortfolioTreeWidget::portfolioChanged(const Portfolio& portfolio)
{
    qInfo() << portfolio.positions.size();
}

void PortfolioTreeWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Name",          ui->treeWidget->columnWidth(PORTFOLIO_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Available",     ui->treeWidget->columnWidth(PORTFOLIO_AVAILABLE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Price",         ui->treeWidget->columnWidth(PORTFOLIO_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_AvgPrice",      ui->treeWidget->columnWidth(PORTFOLIO_AVG_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Cost",          ui->treeWidget->columnWidth(PORTFOLIO_COST_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Part",          ui->treeWidget->columnWidth(PORTFOLIO_PART_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Income",        ui->treeWidget->columnWidth(PORTFOLIO_INCOME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_IncomePercent", ui->treeWidget->columnWidth(PORTFOLIO_INCOME_PERCENT_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_InADay",        ui->treeWidget->columnWidth(PORTFOLIO_IN_A_DAY_COLUMN));
    // clang-format on
}

// NOLINTBEGIN(readability-magic-numbers)
void PortfolioTreeWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->treeWidget->setColumnWidth(PORTFOLIO_NAME_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Name",          COLUMN_WIDTHS[PORTFOLIO_NAME_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_AVAILABLE_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Available",     COLUMN_WIDTHS[PORTFOLIO_AVAILABLE_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_PRICE_COLUMN,          mSettingsEditor->value(type + "/columnWidth_Price",         COLUMN_WIDTHS[PORTFOLIO_PRICE_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_AVG_PRICE_COLUMN,      mSettingsEditor->value(type + "/columnWidth_AvgPrice",      COLUMN_WIDTHS[PORTFOLIO_AVG_PRICE_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_COST_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Cost",          COLUMN_WIDTHS[PORTFOLIO_COST_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_PART_COLUMN,           mSettingsEditor->value(type + "/columnWidth_Part",          COLUMN_WIDTHS[PORTFOLIO_PART_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_INCOME_COLUMN,         mSettingsEditor->value(type + "/columnWidth_Income",        COLUMN_WIDTHS[PORTFOLIO_INCOME_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_INCOME_PERCENT_COLUMN, mSettingsEditor->value(type + "/columnWidth_IncomePercent", COLUMN_WIDTHS[PORTFOLIO_INCOME_PERCENT_COLUMN]).toInt());
    ui->treeWidget->setColumnWidth(PORTFOLIO_IN_A_DAY_COLUMN,       mSettingsEditor->value(type + "/columnWidth_InADay",        COLUMN_WIDTHS[PORTFOLIO_IN_A_DAY_COLUMN]).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
