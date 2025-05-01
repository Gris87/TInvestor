#include "src/widgets/stockstablewidget/stockstablewidget.h"
#include "ui_stockstablewidget.h"

#include <QDebug>



#ifdef Q_OS_WINDOWS
const int COLUMN_WIDTHS[COLUMN_COUNT] = {99, 61, 139, 157, 86, 120, 83};
#else
const int COLUMN_WIDTHS[COLUMN_COUNT] = {100, 69, 150, 170, 91, 129, 88};
#endif



StocksTableWidget::StocksTableWidget(
    IStocksTableRecordFactory*      stockTableRecordFactory,
    IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
    IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*       orderWavesDialogFactory,
    IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
    IUserStorage*                   userStorage,
    IOrderBookThread*               orderBookThread,
    IHttpClient*                    httpClient,
    ISettingsEditor*                settingsEditor,
    QWidget*                        parent
) :
    IStocksTableWidget(parent),
    ui(new Ui::StocksTableWidget),
    tableRecords(),
    lastPricesUpdates(),
    mStocksTableRecordFactory(stockTableRecordFactory),
    mStockTableItemWidgetFactory(stockTableItemWidgetFactory),
    mActionsTableItemWidgetFactory(actionsTableItemWidgetFactory),
    mOrderWavesDialogFactory(orderWavesDialogFactory),
    mOrderWavesWidgetFactory(orderWavesWidgetFactory),
    mUserStorage(userStorage),
    mOrderBookThread(orderBookThread),
    mHttpClient(httpClient),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create StocksTableWidget";

    ui->setupUi(this);

    ui->tableWidget->sortByColumn(STOCK_COLUMN, Qt::AscendingOrder);
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
        const QString uid = stock->meta.uid;
        stock->mutex->unlock();

        IStocksTableRecord* record = tableRecords[uid]; // clazy:exclude=detaching-member

        if (record == nullptr)
        {
            record = mStocksTableRecordFactory->newInstance(
                ui->tableWidget,
                mStockTableItemWidgetFactory,
                mActionsTableItemWidgetFactory,
                mOrderWavesDialogFactory,
                mOrderWavesWidgetFactory,
                mUserStorage,
                mOrderBookThread,
                mHttpClient,
                stock,
                this
            );
            tableRecords[uid] = record;
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
    ui->tableWidget->horizontalHeaderItem(DATE_CHANGE_COLUMN)->setToolTip(tooltip);
}

void StocksTableWidget::lastPriceChanged(const QString& uid)
{
    lastPricesUpdates.insert(uid);
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

void StocksTableWidget::saveWindowState(const QString& type)
{
    // clang-format off
    mSettingsEditor->setValue(type + "/columnWidth_Stock",      ui->tableWidget->columnWidth(STOCK_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Price",      ui->tableWidget->columnWidth(PRICE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_DayChange",  ui->tableWidget->columnWidth(DAY_CHANGE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_DateChange", ui->tableWidget->columnWidth(DATE_CHANGE_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Turnover",   ui->tableWidget->columnWidth(TURNOVER_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Payback",    ui->tableWidget->columnWidth(PAYBACK_COLUMN));
    mSettingsEditor->setValue(type + "/columnWidth_Actions",    ui->tableWidget->columnWidth(ACTIONS_COLUMN));
    // clang-format on
}

// NOLINTBEGIN(readability-magic-numbers)
void StocksTableWidget::loadWindowState(const QString& type)
{
    // clang-format off
    ui->tableWidget->setColumnWidth(STOCK_COLUMN,       mSettingsEditor->value(type + "/columnWidth_Stock",      COLUMN_WIDTHS[STOCK_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PRICE_COLUMN,       mSettingsEditor->value(type + "/columnWidth_Price",      COLUMN_WIDTHS[PRICE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(DAY_CHANGE_COLUMN,  mSettingsEditor->value(type + "/columnWidth_DayChange",  COLUMN_WIDTHS[DAY_CHANGE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(DATE_CHANGE_COLUMN, mSettingsEditor->value(type + "/columnWidth_DateChange", COLUMN_WIDTHS[DATE_CHANGE_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(TURNOVER_COLUMN,    mSettingsEditor->value(type + "/columnWidth_Turnover",   COLUMN_WIDTHS[TURNOVER_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(PAYBACK_COLUMN,     mSettingsEditor->value(type + "/columnWidth_Payback",    COLUMN_WIDTHS[PAYBACK_COLUMN]).toInt());
    ui->tableWidget->setColumnWidth(ACTIONS_COLUMN,     mSettingsEditor->value(type + "/columnWidth_Actions",    COLUMN_WIDTHS[ACTIONS_COLUMN]).toInt());
    // clang-format on
}
// NOLINTEND(readability-magic-numbers)
