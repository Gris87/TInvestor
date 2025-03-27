#include "src/widgets/tablerecord/tablerecord.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPushButton>



TableRecord::TableRecord(
    QTableWidget*                   tableWidget,
    IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
    IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*       orderWavesDialogFactory,
    IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
    IUserStorage*                   userStorage,
    IOrderBookThread*               orderBookThread,
    IHttpClient*                    httpClient,
    Stock*                          stock,
    QObject*                        parent
) :
    ITableRecord(parent),
    mStock(stock),
    mStockTableItemWidget(),
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

    mStockTableItemWidget = stockTableItemWidgetFactory->newInstance(userStorage, tableWidget); // tableWidget will take ownership

    IActionsTableItemWidget* actionsTableItemWidget = actionsTableItemWidgetFactory->newInstance(
        orderWavesDialogFactory, orderWavesWidgetFactory, orderBookThread, httpClient, mStock, mPrecision, tableWidget
    ); // tableWidget will take ownership

    QIcon stockLogo(QString("%1/data/stocks/logos/%2.png").arg(qApp->applicationDirPath(), uid));
    mStockTableItemWidget->setIcon(stockLogo);

    int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    tableWidget->setCellWidget(rowIndex, STOCK_COLUMN, mStockTableItemWidget);
    tableWidget->setItem(rowIndex, PRICE_COLUMN, mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex, DAY_CHANGE_COLUMN, mDayChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, DATE_CHANGE_COLUMN, mDateChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, PAYBACK_COLUMN, mPaybackTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, ACTIONS_COLUMN, actionsTableItemWidget);

    updateAll();
}

TableRecord::~TableRecord()
{
    qDebug() << "Destroy TableRecord";
}

void TableRecord::updateAll()
{
    mStock->mutex->lock();
    mStockTableItemWidget->setQualInvestor(mStock->meta.forQualInvestorFlag);
    mStockTableItemWidget->setText(mStock->meta.ticker);
    mStockTableItemWidget->setFullText(mStock->meta.name);
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
    int  row    = mPriceTableWidgetItem->row();
    bool hidden = !filter.isFiltered(
        mStockTableItemWidget->text(),
        mPriceTableWidgetItem->getValue(),
        mDayChangeTableWidgetItem->getValue(),
        mDateChangeTableWidgetItem->getValue(),
        mPaybackTableWidgetItem->getValue()
    );

    tableWidget->setRowHidden(row, hidden);
}
