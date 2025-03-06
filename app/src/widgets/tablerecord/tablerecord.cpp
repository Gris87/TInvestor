#include "src/widgets/tablerecord/tablerecord.h"

#include <QCoreApplication>
#include <QDebug>



TableRecord::TableRecord(QTableWidget* tableWidget, Stock* stock, QObject* parent) :
    ITableRecord(parent),
    mStock(stock),
    mStockTableWidgetItem(new QTableWidgetItem()),
    mPriceTableWidgetItem(new QTableWidgetItem()),
    mDayChangeTableWidgetItem(new QTableWidgetItem()),
    mDateChangeTableWidgetItem(new QTableWidgetItem()),
    mPaybackTableWidgetItem(new QTableWidgetItem()),
    mLinkTableWidgetItem(new QTableWidgetItem())
{
    qDebug() << "Create TableRecord";

    mStock->mutex->lock();
    QString uid = mStock->meta.uid;
    mStock->mutex->unlock();

    QIcon stockLogo(QString("%1/data/stocks/logos/%2.png").arg(qApp->applicationDirPath(), uid));
    mStockTableWidgetItem->setIcon(stockLogo);

    int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    tableWidget->setItem(rowIndex, STOCK_COLUMN, mStockTableWidgetItem);
    tableWidget->setItem(rowIndex, PRICE_COLUMN, mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex, DAY_CHANGE_COLUMN, mDayChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, DATE_CHANGE_COLUMN, mDateChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, PAYBACK_COLUMN, mPaybackTableWidgetItem);
    tableWidget->setItem(rowIndex, LINK_COLUMN, mLinkTableWidgetItem);

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
    mStock->mutex->unlock();

    updatePrice();
}

void TableRecord::updatePrice()
{
    QMutexLocker lock(mStock->mutex);

    float price = !mStock->operational.detailedData.isEmpty() ? mStock->operational.detailedData.last().price
                                                              : (!mStock->data.isEmpty() ? mStock->data.last().price : 0);

    mPriceTableWidgetItem->setData(Qt::EditRole, price);
}
