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

const QColor HEADER_BACKGROUND_COLOR = QColor("#4F81BD"); // clazy:exclude=non-pod-global-static
const QColor HEADER_FONT_COLOR       = QColor("#FFFFFF"); // clazy:exclude=non-pod-global-static

// constexpr double COLUMN_GAP = 0.71;



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
    IMessageBoxUtils*                  messageBoxUtils,
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
    mMessageBoxUtils(messageBoxUtils),
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

    for (auto it = tableRecords.cbegin(); it != tableRecords.cend(); ++it)
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

    for (auto it = tableRecords.cbegin(); it != tableRecords.cend(); ++it)
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

    for (auto it = tableRecords.cbegin(); it != tableRecords.cend(); ++it)
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

    for (auto it = tableRecords.cbegin(); it != tableRecords.cend(); ++it)
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
    QString lastFile = mSettingsEditor->value("MainWindow/StocksTableWidget/exportToExcelFile", "").toString();

    std::shared_ptr<IFileDialog> fileDialog = mFileDialogFactory->newInstance(
        this, tr("Export"), lastFile.left(lastFile.lastIndexOf("/")), tr("Excel file") + " (*.xlsx)"
    );
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);

    fileDialog->selectFile(lastFile);

    if (fileDialog->exec())
    {
        QString path = fileDialog->selectedFiles().at(0);
        mSettingsEditor->setValue("MainWindow/StocksTableWidget/exportToExcelFile", path);

        exportToExcel(path);
    }
}

void StocksTableWidget::exportToExcel(const QString& path)
{
    QXlsx::Document doc;
    doc.addSheet(tr("Stocks"));

    QXlsx::Format headerStyle;
    headerStyle.setFontBold(true);
    headerStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    headerStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    headerStyle.setFillPattern(QXlsx::Format::PatternSolid);
    headerStyle.setPatternBackgroundColor(HEADER_BACKGROUND_COLOR);
    headerStyle.setFontColor(HEADER_FONT_COLOR);

    for (int i = 0; i < ui->tableWidget->columnCount(); ++i)
    {
        doc.write(1, i + 1, ui->tableWidget->horizontalHeaderItem(i)->text(), headerStyle);
    }

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
