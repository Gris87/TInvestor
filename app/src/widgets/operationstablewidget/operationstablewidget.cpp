#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <QDebug>



// TODO: Move to OperationsTableRecord
enum OperationsTableColumns : qint8
{
    OPERATIONS_TIME_COLUMN,
    OPERATIONS_NAME_COLUMN,
    OPERATIONS_OPERATION_COLUMN,
    OPERATIONS_PRICE_COLUMN,
    OPERATIONS_QUANTITY_COLUMN,
    OPERATIONS_COST_COLUMN,
};



OperationsTableWidget::OperationsTableWidget(ISettingsEditor* settingsEditor, QWidget* parent) :
    IOperationsTableWidget(parent),
    ui(new Ui::OperationsTableWidget),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create OperationsTableWidget";

    ui->setupUi(this);
}

OperationsTableWidget::~OperationsTableWidget()
{
    qDebug() << "Destroy OperationsTableWidget";

    delete ui;
}

void OperationsTableWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Time",      ui->tableWidget->columnWidth(OPERATIONS_TIME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Name",      ui->tableWidget->columnWidth(OPERATIONS_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Operation", ui->tableWidget->columnWidth(OPERATIONS_OPERATION_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Price",     ui->tableWidget->columnWidth(OPERATIONS_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Quantity",  ui->tableWidget->columnWidth(OPERATIONS_QUANTITY_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Cost",      ui->tableWidget->columnWidth(OPERATIONS_COST_COLUMN));
    // clang-format on
}

// NOLINTBEGIN(readability-magic-numbers)
void OperationsTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(OPERATIONS_TIME_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Time",      42).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_NAME_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Name",      60).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_OPERATION_COLUMN, mSettingsEditor->value(type + "/columnWidth_Operation", 63).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_PRICE_COLUMN,     mSettingsEditor->value(type + "/columnWidth_Price",     34).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_QUANTITY_COLUMN,  mSettingsEditor->value(type + "/columnWidth_Quantity",  73).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_COST_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Cost",      65).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
