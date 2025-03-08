#include "src/widgets/tablerecord/tablerecord.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QPushButton>



TableRecord::TableRecord(QTableWidget* tableWidget, Stock* stock, QObject* parent) :
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
    QString uid = mStock->meta.uid;
    qint32  priceNanos = mStock->meta.minPriceIncrement.nano;
    mStock->mutex->unlock();

    QIcon stockLogo(QString("%1/data/stocks/logos/%2.png").arg(qApp->applicationDirPath(), uid));
    mStockTableWidgetItem->setIcon(stockLogo);

    QPushButton* linkButton = new QPushButton(QIcon(":/assets/images/link.png"), ""); // tableWidget will take ownership
    connect(linkButton, SIGNAL(clicked()), this, SLOT(linkButtonClicked()));

    int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    tableWidget->setItem(rowIndex, STOCK_COLUMN, mStockTableWidgetItem);
    tableWidget->setItem(rowIndex, PRICE_COLUMN, mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex, DAY_CHANGE_COLUMN, mDayChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, DATE_CHANGE_COLUMN, mDateChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, PAYBACK_COLUMN, mPaybackTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, LINK_COLUMN, linkButton);

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
    mDayChangeTableWidgetItem->setValue(dayChange);
    mDateChangeTableWidgetItem->setValue(dateChange);
}

void TableRecord::updatePayback()
{
    QMutexLocker lock(mStock->mutex);

    mPaybackTableWidgetItem->setValue(mStock->operational.payback);
}

void TableRecord::linkButtonClicked()
{
    mStock->mutex->lock();
    QUrl url(QString("https://www.tbank.ru/invest/stocks/%1/").arg(mStock->meta.ticker));
    mStock->mutex->unlock();

    bool ok = QDesktopServices::openUrl(url);
    Q_ASSERT_X(ok, "TableRecord::linkButtonClicked()", "Failed to open link");
}
