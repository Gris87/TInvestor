#include "src/widgets/tablerecords/stockstablerecord/stockstablerecord.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPushButton>



constexpr int   START_PRECISION = 9;
constexpr int   TENS            = 10;
constexpr float HUNDRED_PROCENT = 100.0f;



StocksTableRecord::StocksTableRecord(
    QTableWidget*                      tableWidget,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
    IOrderWavesDialogFactory*          orderWavesDialogFactory,
    IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
    IUserStorage*                      userStorage,
    IOrderBookThread*                  orderBookThread,
    IHttpClient*                       httpClient,
    Stock*                             stock,
    QObject*                           parent
) :
    IStocksTableRecord(parent),
    mStock(stock),
    mInstrumentTableItemWidget(),
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

    mPrecision = START_PRECISION;

    while (mPrecision > 2)
    {
        if (priceNanos % TENS != 0)
        {
            break;
        }

        priceNanos /= TENS;
        --mPrecision;
    }

    mInstrumentTableItemWidget =
        instrumentTableItemWidgetFactory->newInstance(userStorage, tableWidget); // tableWidget will take ownership

    IActionsTableItemWidget* actionsTableItemWidget = actionsTableItemWidgetFactory->newInstance(
        orderWavesDialogFactory, orderWavesWidgetFactory, orderBookThread, httpClient, mStock, mPrecision, tableWidget
    ); // tableWidget will take ownership

    const int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    tableWidget->setCellWidget(rowIndex, STOCKS_STOCK_COLUMN, mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex, STOCKS_STOCK_COLUMN, mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex, STOCKS_PRICE_COLUMN, mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex, STOCKS_DAY_CHANGE_COLUMN, mDayChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, STOCKS_DATE_CHANGE_COLUMN, mDateChangeTableWidgetItem);
    tableWidget->setItem(rowIndex, STOCKS_TURNOVER_COLUMN, mTurnoverTableWidgetItem);
    tableWidget->setItem(rowIndex, STOCKS_PAYBACK_COLUMN, mPaybackTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, STOCKS_ACTIONS_COLUMN, actionsTableItemWidget);
}

StocksTableRecord::~StocksTableRecord()
{
    qDebug() << "Destroy StocksTableRecord";
}

void StocksTableRecord::updateAll()
{
    mStock->mutex->lock();

    const QIcon stockLogo(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), mStock->meta.uid));

    mInstrumentTableItemWidget->setIcon(stockLogo);
    mInstrumentTableItemWidget->setQualInvestor(mStock->meta.forQualInvestorFlag);
    mInstrumentTableItemWidget->setText(mStock->meta.ticker);
    mInstrumentTableItemWidget->setFullText(mStock->meta.name);

    mStock->mutex->unlock();

    updatePrice();
    updatePeriodicData();
}

void StocksTableRecord::updatePrice()
{
    const QMutexLocker lock(mStock->mutex);

    const float price = mStock->lastPrice();

    const float dayChange  = mStock->operational.dayStartPrice > 0
                                 ? ((price / mStock->operational.dayStartPrice) * HUNDRED_PROCENT) - HUNDRED_PROCENT
                                 : 0;
    const float dateChange = mStock->operational.specifiedDatePrice > 0
                                 ? ((price / mStock->operational.specifiedDatePrice) * HUNDRED_PROCENT) - HUNDRED_PROCENT
                                 : 0;

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
    const int row = mPriceTableWidgetItem->row();

    const QString text                = mInstrumentTableItemWidget->text();
    const QString fullText            = mInstrumentTableItemWidget->fullText();
    const bool    forQualInvestorFlag = mInstrumentTableItemWidget->forQualInvestorFlag();

    const bool hidden = !filter.isFiltered(
        text,
        fullText,
        forQualInvestorFlag,
        mPriceTableWidgetItem->getValue(),
        mDayChangeTableWidgetItem->getValue(),
        mDateChangeTableWidgetItem->getValue(),
        mTurnoverTableWidgetItem->getValue(),
        mPaybackTableWidgetItem->getValue()
    );

    tableWidget->setRowHidden(row, hidden);
}
