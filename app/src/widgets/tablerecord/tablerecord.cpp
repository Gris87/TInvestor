#include "src/widgets/tablerecord/tablerecord.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPushButton>



TableRecord::TableRecord(
    QTableWidget*                   tableWidget,
    IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*      orderWavesDialogFactory,
    IOrderBookThread*             orderBookThread,
    IHttpClient*                    httpClient,
    Stock*                          stock,
    QObject*                        parent
) :
    ITableRecord(parent),
    mStock(stock),
    mStockTableWidgetItem(new QTableWidgetItem()),
    mPriceTableWidgetItem(new PriceTableItem()),
    mDayChangeTableWidgetItem(new PriceChangeTableItem()),
    mDateChangeTableWidgetItem(new PriceChangeTableItem()),
    mPaybackTableWidgetItem(new PaybackTableItem())
{
    qDebug() << "Create TableRecord";

    mStock->mutex->lock();
    QString uid        = mStock->meta.uid;
    qint32  priceNanos = mStock->meta.minPriceIncrement.nano;
    mStock->mutex->unlock();

    QIcon stockLogo(QString("%1/data/stocks/logos/%2.png").arg(qApp->applicationDirPath(), uid));
    mStockTableWidgetItem->setIcon(stockLogo);

    IActionsTableItemWidget* actionsTableItemWidget = actionsTableItemWidgetFactory->newInstance(
        orderWavesDialogFactory, orderBookThread, httpClient, mStock, tableWidget
    ); // tableWidget will take ownership

    int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    tableWidget->setItem(rowIndex, STOCK_COLUMN, mStockTableWidgetItem);
    tableWidget->setItem(rowIndex, PRICE_COLUMN, mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex, DAY_CHANGE_COLUMN, mDayChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, DATE_CHANGE_COLUMN, mDateChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, PAYBACK_COLUMN, mPaybackTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, ACTIONS_COLUMN, actionsTableItemWidget);

    mPrecision = 9;

    while (mPrecision > 2)
    {
        if (priceNanos % 10 != 0)
        {
            break;
        }

        priceNanos /= 10;
        --mPrecision;
    }

    updateAll();
}

TableRecord::~TableRecord()
{
    qDebug() << "Destroy TableRecord";
}

void TableRecord::updateAll()
{
    mStock->mutex->lock();
    mStockTableWidgetItem->setText(mStock->meta.ticker);
    mStockTableWidgetItem->setToolTip(mStock->meta.name);
    mStock->mutex->unlock();

    updatePrice();
    updatePayback();
}

void TableRecord::updatePrice()
{
    QMutexLocker lock(mStock->mutex);

    float price = !mStock->operational.detailedData.isEmpty() ? mStock->operational.detailedData.last().price
                                                              : (!mStock->data.isEmpty() ? mStock->data.last().price : 0);

    float dayChange = mStock->operational.dayStartPrice > 0 ? (price / mStock->operational.dayStartPrice) * 100 - 100 : 0;
    float dateChange =
        mStock->operational.specifiedDatePrice > 0 ? (price / mStock->operational.specifiedDatePrice) * 100 - 100 : 0;

    mPriceTableWidgetItem->setValue(price, mPrecision);
    mDayChangeTableWidgetItem->setValue(dayChange, mStock->operational.dayStartPrice, mPrecision);
    mDateChangeTableWidgetItem->setValue(dateChange, mStock->operational.specifiedDatePrice, mPrecision);
}

void TableRecord::updatePayback()
{
    QMutexLocker lock(mStock->mutex);

    mPaybackTableWidgetItem->setValue(mStock->operational.payback);
}

void TableRecord::filter(QTableWidget* tableWidget, const Filter& filter)
{
    int  row    = mStockTableWidgetItem->row();
    bool hidden = !filter.isFiltered(
        mStockTableWidgetItem->text(),
        mPriceTableWidgetItem->getValue(),
        mDayChangeTableWidgetItem->getValue(),
        mDateChangeTableWidgetItem->getValue(),
        mPaybackTableWidgetItem->getValue()
    );

    tableWidget->setRowHidden(row, hidden);
}
