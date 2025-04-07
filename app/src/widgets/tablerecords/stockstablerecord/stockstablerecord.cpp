#include "src/widgets/tablerecords/stockstablerecord/stockstablerecord.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPushButton>



StocksTableRecord::StocksTableRecord(
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
    IStocksTableRecord(parent),
    mStock(stock),
    mStockTableItemWidget(),
    mPriceTableWidgetItem(new PriceTableItem()),
    mDayChangeTableWidgetItem(new PriceChangeTableItem()),
    mDateChangeTableWidgetItem(new PriceChangeTableItem()),
    mTurnoverTableWidgetItem(new TurnoverTableItem()),
    mPaybackTableWidgetItem(new PaybackTableItem())
{
    qDebug() << "Create StocksTableRecord";

    mStock->mutex->lock();
    qint32 priceNanos = mStock->meta.minPriceIncrement.nano;
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

    int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    tableWidget->setCellWidget(rowIndex, STOCK_COLUMN, mStockTableItemWidget);
    tableWidget->setItem(rowIndex, STOCK_COLUMN, mStockTableItemWidget);
    tableWidget->setItem(rowIndex, PRICE_COLUMN, mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex, DAY_CHANGE_COLUMN, mDayChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, DATE_CHANGE_COLUMN, mDateChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, TURNOVER_COLUMN, mTurnoverTableWidgetItem);
    tableWidget->setItem(rowIndex, PAYBACK_COLUMN, mPaybackTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, ACTIONS_COLUMN, actionsTableItemWidget);

    updateAll();
}

StocksTableRecord::~StocksTableRecord()
{
    qDebug() << "Destroy StocksTableRecord";
}

void StocksTableRecord::updateAll()
{
    mStock->mutex->lock();

    QIcon stockLogo(QString("%1/data/stocks/logos/%2.png").arg(qApp->applicationDirPath(), mStock->meta.uid));

    mStockTableItemWidget->setIcon(stockLogo);
    mStockTableItemWidget->setQualInvestor(mStock->meta.forQualInvestorFlag);
    mStockTableItemWidget->setText(mStock->meta.ticker);
    mStockTableItemWidget->setFullText(mStock->meta.name);

    mStock->mutex->unlock();

    updatePrice();
    updatePeriodicData();
}

void StocksTableRecord::updatePrice()
{
    const QMutexLocker lock(mStock->mutex);

    float price = !mStock->operational.detailedData.isEmpty() ? mStock->operational.detailedData.last().price
                                                              : (!mStock->data.isEmpty() ? mStock->data.last().price : 0);

    float dayChange = mStock->operational.dayStartPrice > 0 ? (price / mStock->operational.dayStartPrice) * 100 - 100 : 0;
    float dateChange =
        mStock->operational.specifiedDatePrice > 0 ? (price / mStock->operational.specifiedDatePrice) * 100 - 100 : 0;

    mPriceTableWidgetItem->setValue(price, mPrecision);
    mDayChangeTableWidgetItem->setValue(dayChange, mStock->operational.dayStartPrice, mPrecision);
    mDateChangeTableWidgetItem->setValue(dateChange, mStock->operational.specifiedDatePrice, mPrecision);
}

void StocksTableRecord::updatePeriodicData()
{
    const QMutexLocker lock(mStock->mutex);

    mTurnoverTableWidgetItem->setValue(mStock->operational.turnover);
    mPaybackTableWidgetItem->setValue(mStock->operational.payback);
}

void StocksTableRecord::filter(QTableWidget* tableWidget, const Filter& filter)
{
    int  row    = mPriceTableWidgetItem->row();
    bool hidden = !filter.isFiltered(
        mStockTableItemWidget->text(),
        mStockTableItemWidget->fullText(),
        mStockTableItemWidget->forQualInvestorFlag(),
        mPriceTableWidgetItem->getValue(),
        mDayChangeTableWidgetItem->getValue(),
        mDateChangeTableWidgetItem->getValue(),
        mTurnoverTableWidgetItem->getValue(),
        mPaybackTableWidgetItem->getValue()
    );

    tableWidget->setRowHidden(row, hidden);
}
