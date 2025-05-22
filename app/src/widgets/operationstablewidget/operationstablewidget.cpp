#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <QDebug>
#include <QMenu>

#include "src/qxlsx/xlsxdocument.h"



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[OPERATIONS_COLUMN_COUNT] = {110, 133, 309, 56, 106, 95, 167, 87, 85, 95, 173, 194, 115, 95};
#else
const int COLUMN_WIDTHS[OPERATIONS_COLUMN_COUNT] = {121, 138, 154, 59, 114, 102, 180, 95, 90, 101, 184, 206, 125, 101};
#endif

const QColor HEADER_BACKGROUND_COLOR = QColor("#354450"); // clazy:exclude=non-pod-global-static
const QColor HEADER_FONT_COLOR       = QColor("#699BA2"); // clazy:exclude=non-pod-global-static

constexpr double COLUMN_GAP = 0.71;



OperationsTableWidget::OperationsTableWidget(
    IOperationsTableRecordFactory*     operationsTableRecordFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    IFileDialogFactory*                fileDialogFactory,
    IMessageBoxUtils*                  messageBoxUtils,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) :
    IOperationsTableWidget(parent),
    ui(new Ui::OperationsTableWidget),
    mOperationsTableRecordFactory(operationsTableRecordFactory),
    mInstrumentTableItemWidgetFactory(instrumentTableItemWidgetFactory),
    mUserStorage(userStorage),
    mInstrumentsStorage(instrumentsStorage),
    mFileDialogFactory(fileDialogFactory),
    mMessageBoxUtils(messageBoxUtils),
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
            ui->tableWidget, mInstrumentTableItemWidgetFactory, mUserStorage, mInstrumentsStorage, this
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
            ui->tableWidget, mInstrumentTableItemWidgetFactory, mUserStorage, mInstrumentsStorage, this
        );
        record->setOperation(operation);

        mTableRecords.append(record);
    }

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
}

void OperationsTableWidget::on_tableWidget_customContextMenuRequested(const QPoint& pos)
{
    QMenu* contextMenu = new QMenu(this);

    contextMenu->addAction(tr("Export to Excel"), this, SLOT(actionExportToExcelTriggered()));

    contextMenu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

void OperationsTableWidget::actionExportToExcelTriggered()
{
    const QString lastFile = mSettingsEditor->value("MainWindow/OperationsTableWidget/exportToExcelFile", "").toString();

    const std::shared_ptr<IFileDialog> fileDialog = mFileDialogFactory->newInstance(
        this, tr("Export"), lastFile.left(lastFile.lastIndexOf("/")), tr("Excel file") + " (*.xlsx)"
    );
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDefaultSuffix("xlsx");

    fileDialog->selectFile(lastFile);

    if (fileDialog->exec() == QDialog::Accepted)
    {
        const QString path = fileDialog->selectedFiles().at(0);
        mSettingsEditor->setValue("MainWindow/OperationsTableWidget/exportToExcelFile", path);

        exportToExcel(path);
    }
}

void OperationsTableWidget::exportToExcel(const QString& path) const
{
    QXlsx::Document doc;
    doc.addSheet(tr("Operations"));

    QXlsx::Format headerStyle;
    headerStyle.setFontBold(true);
    headerStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    headerStyle.setFillPattern(QXlsx::Format::PatternSolid);
    headerStyle.setBorderStyle(QXlsx::Format::BorderThin);
    headerStyle.setPatternBackgroundColor(HEADER_BACKGROUND_COLOR);
    headerStyle.setFontColor(HEADER_FONT_COLOR);

    for (int i = 0; i < ui->tableWidget->columnCount(); ++i)
    {
        doc.write(1, i + 1, ui->tableWidget->horizontalHeaderItem(i)->text(), headerStyle);
    }

    for (IOperationsTableRecord* record : std::as_const(mTableRecords))
    {
        record->exportToExcel(doc);
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.setColumnWidth(OPERATIONS_TIME_COLUMN + 1,                          17.57 + COLUMN_GAP);
    doc.autosizeColumnWidth(OPERATIONS_NAME_COLUMN + 1);
    doc.autosizeColumnWidth(OPERATIONS_DESCRIPTION_COLUMN + 1);
    doc.setColumnWidth(OPERATIONS_PRICE_COLUMN + 1,                         7.43  + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_AVG_PRICE_COLUMN + 1,                     13.14 + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_QUANTITY_COLUMN + 1,                      10.86 + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_REMAINED_QUANTITY_COLUMN + 1,             22.43 + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_PAYMENT_COLUMN + 1,                       11.43 + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_COMMISSION_COLUMN + 1,                    9.14  + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_YIELD_COLUMN + 1,                         11.29 + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_YIELD_WITH_COMMISSION_COLUMN + 1,         23.43 + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN + 1, 27    + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_REMAINED_MONEY_COLUMN + 1,                14.57 + COLUMN_GAP);
    doc.setColumnWidth(OPERATIONS_TOTAL_MONEY_COLUMN + 1,                   11.14 + COLUMN_GAP);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.saveAs(path);
}

void OperationsTableWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Time",                       ui->tableWidget->columnWidth(OPERATIONS_TIME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Name",                       ui->tableWidget->columnWidth(OPERATIONS_NAME_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Description",                ui->tableWidget->columnWidth(OPERATIONS_DESCRIPTION_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Price",                      ui->tableWidget->columnWidth(OPERATIONS_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_AvgPrice",                   ui->tableWidget->columnWidth(OPERATIONS_AVG_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Quantity",                   ui->tableWidget->columnWidth(OPERATIONS_QUANTITY_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_RemainedQuantity",           ui->tableWidget->columnWidth(OPERATIONS_REMAINED_QUANTITY_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Payment",                    ui->tableWidget->columnWidth(OPERATIONS_PAYMENT_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Commission",                 ui->tableWidget->columnWidth(OPERATIONS_COMMISSION_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Yield",                      ui->tableWidget->columnWidth(OPERATIONS_YIELD_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_YieldWithCommission",        ui->tableWidget->columnWidth(OPERATIONS_YIELD_WITH_COMMISSION_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_YieldWithCommissionPercent", ui->tableWidget->columnWidth(OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_RemainedMoney",              ui->tableWidget->columnWidth(OPERATIONS_REMAINED_MONEY_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_TotalMoney",                 ui->tableWidget->columnWidth(OPERATIONS_TOTAL_MONEY_COLUMN));
    // clang-format on
}

// NOLINTBEGIN(readability-magic-numbers)
void OperationsTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(OPERATIONS_TIME_COLUMN,                          mSettingsEditor->value(type + "/columnWidth_Time",                       COLUMN_WIDTHS[OPERATIONS_TIME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_NAME_COLUMN,                          mSettingsEditor->value(type + "/columnWidth_Name",                       COLUMN_WIDTHS[OPERATIONS_NAME_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_DESCRIPTION_COLUMN,                   mSettingsEditor->value(type + "/columnWidth_Description",                COLUMN_WIDTHS[OPERATIONS_DESCRIPTION_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_PRICE_COLUMN,                         mSettingsEditor->value(type + "/columnWidth_Price",                      COLUMN_WIDTHS[OPERATIONS_PRICE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_AVG_PRICE_COLUMN,                     mSettingsEditor->value(type + "/columnWidth_AvgPrice",                   COLUMN_WIDTHS[OPERATIONS_AVG_PRICE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_QUANTITY_COLUMN,                      mSettingsEditor->value(type + "/columnWidth_Quantity",                   COLUMN_WIDTHS[OPERATIONS_QUANTITY_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_REMAINED_QUANTITY_COLUMN,             mSettingsEditor->value(type + "/columnWidth_RemainedQuantity",           COLUMN_WIDTHS[OPERATIONS_REMAINED_QUANTITY_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_PAYMENT_COLUMN,                       mSettingsEditor->value(type + "/columnWidth_Payment",                    COLUMN_WIDTHS[OPERATIONS_PAYMENT_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_COMMISSION_COLUMN,                    mSettingsEditor->value(type + "/columnWidth_Commission",                 COLUMN_WIDTHS[OPERATIONS_COMMISSION_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_YIELD_COLUMN,                         mSettingsEditor->value(type + "/columnWidth_Yield",                      COLUMN_WIDTHS[OPERATIONS_YIELD_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_YIELD_WITH_COMMISSION_COLUMN,         mSettingsEditor->value(type + "/columnWidth_YieldWithCommission",        COLUMN_WIDTHS[OPERATIONS_YIELD_WITH_COMMISSION_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN, mSettingsEditor->value(type + "/columnWidth_YieldWithCommissionPercent", COLUMN_WIDTHS[OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_REMAINED_MONEY_COLUMN,                mSettingsEditor->value(type + "/columnWidth_RemainedMoney",              COLUMN_WIDTHS[OPERATIONS_REMAINED_MONEY_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(OPERATIONS_TOTAL_MONEY_COLUMN,                   mSettingsEditor->value(type + "/columnWidth_TotalMoney",                 COLUMN_WIDTHS[OPERATIONS_TOTAL_MONEY_COLUMN]).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
