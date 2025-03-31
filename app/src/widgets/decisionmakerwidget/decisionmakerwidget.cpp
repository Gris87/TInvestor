#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <QDebug>



// TODO: Move to TableRecord
#define NAME_COLUMN           0
#define AVAILABLE_COLUMN      1
#define PRICE_COLUMN          2
#define AVG_PRICE_COLUMN      3
#define COST_COLUMN           4
#define PART_COLUMN           5
#define INCOME_COLUMN         6
#define INCOME_PERCENT_COLUMN 7
#define IN_A_DAY_COLUMN       8



DecisionMakerWidget::DecisionMakerWidget(ISettingsEditor* settingsEditor, const QString& type, QWidget* parent) :
    IDecisionMakerWidget(parent),
    ui(new Ui::DecisionMakerWidget),
    mSettingsEditor(settingsEditor),
    mType(type)
{
    qDebug() << "Create DecisionMakerWidget";

    ui->setupUi(this);
}

DecisionMakerWidget::~DecisionMakerWidget()
{
    qDebug() << "Destroy DecisionMakerWidget";

    delete ui;
}

void DecisionMakerWidget::saveWindowState()
{
    QString type = "MainWindow/" + mType;

    // clang-format off
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Name",          ui->portfolioTableWidget->columnWidth(NAME_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Available",     ui->portfolioTableWidget->columnWidth(AVAILABLE_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Price",         ui->portfolioTableWidget->columnWidth(PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_AvgPrice",      ui->portfolioTableWidget->columnWidth(AVG_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Cost",          ui->portfolioTableWidget->columnWidth(COST_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Part",          ui->portfolioTableWidget->columnWidth(PART_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_Income",        ui->portfolioTableWidget->columnWidth(INCOME_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_IncomePercent", ui->portfolioTableWidget->columnWidth(INCOME_PERCENT_COLUMN));
    mSettingsEditor->setValue(type + "/portfolioTableWidget_InADay",        ui->portfolioTableWidget->columnWidth(IN_A_DAY_COLUMN));
    // clang-format on
}

void DecisionMakerWidget::loadWindowState()
{
    QString type = "MainWindow/" + mType;

    // clang-format off
    ui->portfolioTableWidget->setColumnWidth(NAME_COLUMN,           mSettingsEditor->value(type + "/portfolioTableWidget_Name",          60).toInt());
    ui->portfolioTableWidget->setColumnWidth(AVAILABLE_COLUMN,      mSettingsEditor->value(type + "/portfolioTableWidget_Available",     58).toInt());
    ui->portfolioTableWidget->setColumnWidth(PRICE_COLUMN,          mSettingsEditor->value(type + "/portfolioTableWidget_Price",         34).toInt());
    ui->portfolioTableWidget->setColumnWidth(AVG_PRICE_COLUMN,      mSettingsEditor->value(type + "/portfolioTableWidget_AvgPrice",      84).toInt());
    ui->portfolioTableWidget->setColumnWidth(COST_COLUMN,           mSettingsEditor->value(type + "/portfolioTableWidget_Cost",          65).toInt());
    ui->portfolioTableWidget->setColumnWidth(PART_COLUMN,           mSettingsEditor->value(type + "/portfolioTableWidget_Part",          34).toInt());
    ui->portfolioTableWidget->setColumnWidth(INCOME_COLUMN,         mSettingsEditor->value(type + "/portfolioTableWidget_Income",        41).toInt());
    ui->portfolioTableWidget->setColumnWidth(INCOME_PERCENT_COLUMN, mSettingsEditor->value(type + "/portfolioTableWidget_IncomePercent", 57).toInt());
    ui->portfolioTableWidget->setColumnWidth(IN_A_DAY_COLUMN,       mSettingsEditor->value(type + "/portfolioTableWidget_InADay",        64).toInt());
    // clang-format on
}
