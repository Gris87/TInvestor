#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <QDebug>



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[OPERATIONS_COLUMN_COUNT] = {64, 82, 85, 56, 95, 87};
#else
const int COLUMN_WIDTHS[OPERATIONS_COLUMN_COUNT] = {67, 89, 91, 59, 102, 95};
#endif



OperationsTableWidget::OperationsTableWidget(
    IOperationsTableRecordFactory* operationsTableRecordFactory,
    IStockTableItemWidgetFactory*  stockTableItemWidgetFactory,
    IUserStorage*                  userStorage,
    IInstrumentsStorage*           instrumentsStorage,
    ISettingsEditor*               settingsEditor,
    QWidget*                       parent
) :
    IOperationsTableWidget(parent),
    ui(new Ui::OperationsTableWidget),
    mOperationsTableRecordFactory(operationsTableRecordFactory),
    mStockTableItemWidgetFactory(stockTableItemWidgetFactory),
    mUserStorage(userStorage),
    mInstrumentsStorage(instrumentsStorage),
    mSettingsEditor(settingsEditor),
    mTableRecords()
{
    qDebug() << "Create OperationsTableWidget";

    ui->setupUi(this);

    ui->tableWidget->sortByColumn(OPERATIONS_TIME_COLUMN, Qt::DescendingOrder);
}

OperationsTableWidget::~OperationsTableWidget()
{
    qDebug() << "Destroy OperationsTableWidget";

    delete ui;
}

void OperationsTableWidget::operationsRead(const QList<Operation>& operations)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setSortingEnabled(false);

    if (mTableRecords.size() > operations.size())
    {
        while (mTableRecords.size() > operations.size())
        {
            delete mTableRecords.takeLast();
        }

        ui->tableWidget->setRowCount(operations.size());
    }

    while (mTableRecords.size() < operations.size())
    {
        IOperationsTableRecord* record = mOperationsTableRecordFactory->newInstance(
            ui->tableWidget, mStockTableItemWidgetFactory, mUserStorage, mInstrumentsStorage, this
        );
        mTableRecords.append(record);
    }

    for (int i = 0; i < operations.size(); ++i)
    {
        mTableRecords.at(i)->setOperation(operations.at(i));
    }

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
}

void OperationsTableWidget::operationsAdded(const QList<Operation>& operations)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setSortingEnabled(false);

    for (const Operation& operation : operations)
    {
        IOperationsTableRecord* record = mOperationsTableRecordFactory->newInstance(
            ui->tableWidget, mStockTableItemWidgetFactory, mUserStorage, mInstrumentsStorage, this
        );
        record->setOperation(operation);

        mTableRecords.append(record);
    }

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
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
    ui->tableWidget->setColumnWidth(OPERATIONS_TIME_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Time",      COLUMN_WIDTHS[OPERATIONS_TIME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_NAME_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Name",      COLUMN_WIDTHS[OPERATIONS_NAME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_OPERATION_COLUMN, mSettingsEditor->value(type + "/columnWidth_Operation", COLUMN_WIDTHS[OPERATIONS_OPERATION_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_PRICE_COLUMN,     mSettingsEditor->value(type + "/columnWidth_Price",     COLUMN_WIDTHS[OPERATIONS_PRICE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_QUANTITY_COLUMN,  mSettingsEditor->value(type + "/columnWidth_Quantity",  COLUMN_WIDTHS[OPERATIONS_QUANTITY_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_COST_COLUMN,      mSettingsEditor->value(type + "/columnWidth_Cost",      COLUMN_WIDTHS[OPERATIONS_COST_COLUMN]).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
