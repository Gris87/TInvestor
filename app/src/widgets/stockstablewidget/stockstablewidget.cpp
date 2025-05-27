#include "src/widgets/stockstablewidget/stockstablewidget.h"
#include "ui_stockstablewidget.h"

#include <QDebug>
#include <QMenu>

#include "src/qxlsx/xlsxdocument.h"



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[STOCKS_COLUMN_COUNT] = {99, 61, 139, 157, 86, 120, 83};
#else
const int COLUMN_WIDTHS[STOCKS_COLUMN_COUNT] = {100, 69, 150, 170, 91, 129, 88};
#endif

const QColor HEADER_BACKGROUND_COLOR = QColor("#354450"); // clazy:exclude=non-pod-global-static
const QColor HEADER_FONT_COLOR       = QColor("#699BA2"); // clazy:exclude=non-pod-global-static
const QColor CELL_BACKGROUND_COLOR   = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor CELL_FONT_COLOR         = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr double COLUMN_GAP = 0.71;



StocksTableWidget::StocksTableWidget(
    IStocksTableRecordFactory*         stocksTableRecordFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*          orderWavesDialogFactory,
    IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
    IUserStorage*                      userStorage,
    IOrderBookThread*                  orderBookThread,
    IHttpClient*                       httpClient,
    IFileDialogFactory*                fileDialogFactory,
    ISettingsEditor*                   settingsEditor,
    QWidget*                           parent
) :
    IStocksTableWidget(parent),
    ui(new Ui::StocksTableWidget),
    tableRecords(),
    lastPricesUpdates(),
    mStocksTableRecordFactory(stocksTableRecordFactory),
    mInstrumentTableItemWidgetFactory(instrumentTableItemWidgetFactory),
    mActionsTableItemWidgetFactory(actionsTableItemWidgetFactory),
    mOrderWavesDialogFactory(orderWavesDialogFactory),
    mOrderWavesWidgetFactory(orderWavesWidgetFactory),
    mUserStorage(userStorage),
    mOrderBookThread(orderBookThread),
    mHttpClient(httpClient),
    mFileDialogFactory(fileDialogFactory),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create StocksTableWidget";

    ui->setupUi(this);

    ui->tableWidget->sortByColumn(STOCKS_STOCK_COLUMN, Qt::AscendingOrder);
}

StocksTableWidget::~StocksTableWidget()
{
    qDebug() << "Destroy StocksTableWidget";

    delete ui;
}

void StocksTableWidget::updateTable(const QList<Stock*>& stocks, const Filter& filter)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setSortingEnabled(false);

    for (Stock* stock : stocks)
    {
        stock->mutex->lock();
        const QString instrumentId = stock->meta.uid;
        stock->mutex->unlock();

        IStocksTableRecord* record = tableRecords[instrumentId]; // clazy:exclude=detaching-member

        if (record == nullptr)
        {
            record = mStocksTableRecordFactory->newInstance(
                ui->tableWidget,
                mInstrumentTableItemWidgetFactory,
                mActionsTableItemWidgetFactory,
                mOrderWavesDialogFactory,
                mOrderWavesWidgetFactory,
                mUserStorage,
                mOrderBookThread,
                mHttpClient,
                stock,
                this
            );
            tableRecords[instrumentId] = record;
        }

        record->updateAll();
        record->filter(ui->tableWidget, filter);
    }

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
}

void StocksTableWidget::updateAll(const Filter& filter)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setSortingEnabled(false);

    for (auto it = tableRecords.constBegin(); it != tableRecords.constEnd(); ++it)
    {
        it.value()->updateAll();
        it.value()->filter(ui->tableWidget, filter);
    }

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
}

void StocksTableWidget::updateLastPrices(const Filter& filter)
{
    if (!lastPricesUpdates.isEmpty())
    {
        ui->tableWidget->setUpdatesEnabled(false);
        ui->tableWidget->setSortingEnabled(false);

        for (const QString& lastPricesUpdate : std::as_const(lastPricesUpdates))
        {
            IStocksTableRecord* record = tableRecords[lastPricesUpdate]; // clazy:exclude=detaching-member

            if (record != nullptr)
            {
                record->updatePrice();
                record->filter(ui->tableWidget, filter);
            }
        }

        lastPricesUpdates.clear();

        ui->tableWidget->setSortingEnabled(true);
        ui->tableWidget->setUpdatesEnabled(true);
    }
}

void StocksTableWidget::updatePrices(const Filter& filter)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setSortingEnabled(false);

    for (auto it = tableRecords.constBegin(); it != tableRecords.constEnd(); ++it)
    {
        it.value()->updatePrice();
        it.value()->filter(ui->tableWidget, filter);
    }

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
}

void StocksTableWidget::updatePeriodicData(const Filter& filter)
{
    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setSortingEnabled(false);

    for (auto it = tableRecords.constBegin(); it != tableRecords.constEnd(); ++it)
    {
        it.value()->updatePeriodicData();
        it.value()->filter(ui->tableWidget, filter);
    }

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setUpdatesEnabled(true);
}

void StocksTableWidget::setDateChangeTooltip(const QString& tooltip)
{
    ui->tableWidget->horizontalHeaderItem(STOCKS_DATE_CHANGE_COLUMN)->setToolTip(tooltip);
}

void StocksTableWidget::lastPriceChanged(const QString& instrumentId)
{
    lastPricesUpdates.insert(instrumentId);
}

void StocksTableWidget::filterChanged(const Filter& filter)
{
    ui->tableWidget->setUpdatesEnabled(false);

    for (auto it = tableRecords.constBegin(); it != tableRecords.constEnd(); ++it)
    {
        it.value()->filter(ui->tableWidget, filter);
    }

    ui->tableWidget->setUpdatesEnabled(true);
}

void StocksTableWidget::on_tableWidget_customContextMenuRequested(const QPoint& pos)
{
    QMenu* contextMenu = new QMenu(this);

    contextMenu->addAction(tr("Export to Excel"), this, SLOT(actionExportToExcelTriggered()));

    contextMenu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

void StocksTableWidget::actionExportToExcelTriggered()
{
    const QString lastFile = mSettingsEditor->value("MainWindow/StocksTableWidget/exportToExcelFile", "").toString();

    const std::shared_ptr<IFileDialog> fileDialog = mFileDialogFactory->newInstance(
        this, tr("Export"), lastFile.left(lastFile.lastIndexOf("/")), tr("Excel file") + " (*.xlsx)"
    );
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDefaultSuffix("xlsx");

    fileDialog->selectFile(lastFile);

    if (fileDialog->exec() == QDialog::Accepted)
    {
        const QString path = fileDialog->selectedFiles().at(0);
        mSettingsEditor->setValue("MainWindow/StocksTableWidget/exportToExcelFile", path);

        exportToExcel(path);
    }
}

void StocksTableWidget::exportToExcel(const QString& path) const
{
    QXlsx::Document doc;
    doc.addSheet(tr("Stocks"));

    QXlsx::Format headerStyle;
    headerStyle.setFontBold(true);
    headerStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    headerStyle.setFillPattern(QXlsx::Format::PatternSolid);
    headerStyle.setBorderStyle(QXlsx::Format::BorderThin);
    headerStyle.setPatternBackgroundColor(HEADER_BACKGROUND_COLOR);
    headerStyle.setFontColor(HEADER_FONT_COLOR);

    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    doc.write(1, STOCKS_STOCK_COLUMN + 1, ui->tableWidget->horizontalHeaderItem(STOCKS_STOCK_COLUMN)->text(), headerStyle);
    doc.write(1, STOCKS_STOCK_COLUMN + 2, tr("Qual investor"), headerStyle);

    for (int i = STOCKS_PRICE_COLUMN; i < STOCKS_ACTIONS_COLUMN; ++i)
    {
        doc.write(1, i + 2, ui->tableWidget->horizontalHeaderItem(i)->text(), headerStyle);
    }

    doc.write(
        1, STOCKS_PAYBACK_COLUMN + 4, ui->tableWidget->horizontalHeaderItem(STOCKS_DATE_CHANGE_COLUMN)->text(), headerStyle
    );
    doc.write(
        2, STOCKS_PAYBACK_COLUMN + 4, ui->tableWidget->horizontalHeaderItem(STOCKS_DATE_CHANGE_COLUMN)->toolTip(), cellStyle
    );

    for (auto it = tableRecords.constBegin(), end = tableRecords.constEnd(); it != end; ++it)
    {
        it.value()->exportToExcel(doc);
    }

    // NOLINTBEGIN(readability-magic-numbers)
    // clang-format off
    doc.autosizeColumnWidth(STOCKS_STOCK_COLUMN + 1);
    doc.setColumnWidth(STOCKS_STOCK_COLUMN + 2,       13.57 + COLUMN_GAP);
    doc.setColumnWidth(STOCKS_PRICE_COLUMN + 2,       9.43  + COLUMN_GAP);
    doc.setColumnWidth(STOCKS_DAY_CHANGE_COLUMN + 2,  18.57 + COLUMN_GAP);
    doc.setColumnWidth(STOCKS_DATE_CHANGE_COLUMN + 2, 18.86 + COLUMN_GAP);
    doc.setColumnWidth(STOCKS_TURNOVER_COLUMN + 2,    11.86 + COLUMN_GAP);
    doc.setColumnWidth(STOCKS_PAYBACK_COLUMN + 2,     12.57 + COLUMN_GAP);
    doc.setColumnWidth(STOCKS_PAYBACK_COLUMN + 4,     21    + COLUMN_GAP);
    // clang-format on
    // NOLINTEND(readability-magic-numbers)

    doc.saveAs(path);
}

void StocksTableWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Stock",      ui->tableWidget->columnWidth(STOCKS_STOCK_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Price",      ui->tableWidget->columnWidth(STOCKS_PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_DayChange",  ui->tableWidget->columnWidth(STOCKS_DAY_CHANGE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_DateChange", ui->tableWidget->columnWidth(STOCKS_DATE_CHANGE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Turnover",   ui->tableWidget->columnWidth(STOCKS_TURNOVER_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Payback",    ui->tableWidget->columnWidth(STOCKS_PAYBACK_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Actions",    ui->tableWidget->columnWidth(STOCKS_ACTIONS_COLUMN));
    // clang-format on
}

// NOLINTBEGIN(readability-magic-numbers)
void StocksTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(STOCKS_STOCK_COLUMN,       mSettingsEditor->value(type + "/columnWidth_Stock",      COLUMN_WIDTHS[STOCKS_STOCK_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(STOCKS_PRICE_COLUMN,       mSettingsEditor->value(type + "/columnWidth_Price",      COLUMN_WIDTHS[STOCKS_PRICE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(STOCKS_DAY_CHANGE_COLUMN,  mSettingsEditor->value(type + "/columnWidth_DayChange",  COLUMN_WIDTHS[STOCKS_DAY_CHANGE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(STOCKS_DATE_CHANGE_COLUMN, mSettingsEditor->value(type + "/columnWidth_DateChange", COLUMN_WIDTHS[STOCKS_DATE_CHANGE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(STOCKS_TURNOVER_COLUMN,    mSettingsEditor->value(type + "/columnWidth_Turnover",   COLUMN_WIDTHS[STOCKS_TURNOVER_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(STOCKS_PAYBACK_COLUMN,     mSettingsEditor->value(type + "/columnWidth_Payback",    COLUMN_WIDTHS[STOCKS_PAYBACK_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(STOCKS_ACTIONS_COLUMN,     mSettingsEditor->value(type + "/columnWidth_Actions",    COLUMN_WIDTHS[STOCKS_ACTIONS_COLUMN]).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
