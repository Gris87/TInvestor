#include "src/widgets/tablerecords/operationstablerecord/operationstablerecord.h"

#include <QCoreApplication>
#include <QDebug>

#include "src/grpc/utils.h"



OperationsTableRecord::OperationsTableRecord(
    QTableWidget*                      tableWidget,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    QObject*                           parent
) :
    IOperationsTableRecord(parent),
    mTimeTableWidgetItem(new TimeTableItem()),
    mInstrumentTableItemWidget(),
    mDescriptionTableWidgetItem(new QTableWidgetItem()),
    mPriceTableWidgetItem(new PriceTableItem()),
    mAvgPriceTableWidgetItem(new PriceTableItem()),
    mQuantityTableWidgetItem(new IntegerTableItem()),
    mRemainedQuantityTableWidgetItem(new IntegerTableItem()),
    mPaymentTableWidgetItem(new PriceTableItem()),
    mCommissionTableWidgetItem(new PriceTableItem()),
    mYieldTableWidgetItem(new PriceTableItem()),
    mYieldWithCommissionTableWidgetItem(new PriceTableItem()),
    mYieldWithCommissionPercentTableWidgetItem(new PriceChangeTableItem()),
    mRemainedMoneyTableWidgetItem(new PriceTableItem()),
    mTotalMoneyTableWidgetItem(new PriceTableItem()),
    mInstrumentsStorage(instrumentsStorage)
{
    qDebug() << "Create OperationsTableRecord";

    mInstrumentTableItemWidget =
        instrumentTableItemWidgetFactory->newInstance(userStorage, tableWidget); // tableWidget will take ownership

    const int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    // clang-format off
    tableWidget->setItem(rowIndex,       OPERATIONS_TIME_COLUMN,                          mTimeTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, OPERATIONS_NAME_COLUMN,                          mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex,       OPERATIONS_NAME_COLUMN,                          mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex,       OPERATIONS_DESCRIPTION_COLUMN,                   mDescriptionTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_PRICE_COLUMN,                         mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_AVG_PRICE_COLUMN,                     mAvgPriceTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_QUANTITY_COLUMN,                      mQuantityTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_REMAINED_QUANTITY_COLUMN,             mRemainedQuantityTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_PAYMENT_COLUMN,                       mPaymentTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_COMMISSION_COLUMN,                    mCommissionTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_YIELD_COLUMN,                         mYieldTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_YIELD_WITH_COMMISSION_COLUMN,         mYieldWithCommissionTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN, mYieldWithCommissionPercentTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_REMAINED_MONEY_COLUMN,                mRemainedMoneyTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_TOTAL_MONEY_COLUMN,                   mTotalMoneyTableWidgetItem);
    // clang-format on
}

OperationsTableRecord::~OperationsTableRecord()
{
    qDebug() << "Destroy OperationsTableRecord";
}

void OperationsTableRecord::setOperation(const Operation& operation)
{
    const QMutexLocker lock(mInstrumentsStorage->getMutex());

    const Instruments& instruments = mInstrumentsStorage->getInstruments();
    Instrument         instrument  = instruments[operation.instrumentId];

    const QIcon instrumentLogo(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), operation.instrumentId)
    );

    if (instrument.ticker == "")
    {
        instrument.ticker = operation.instrumentId;
    }

    if (instrument.name == "")
    {
        instrument.name = "?????";
    }

    const float avgPrice = quotationToFloat(operation.avgPrice);

    mTimeTableWidgetItem->setValue(QDateTime::fromMSecsSinceEpoch(operation.timestamp));
    mInstrumentTableItemWidget->setIcon(instrumentLogo);
    mInstrumentTableItemWidget->setText(instrument.ticker);
    mInstrumentTableItemWidget->setFullText(instrument.name);
    mDescriptionTableWidgetItem->setText(operation.description);
    mPriceTableWidgetItem->setValue(operation.price, operation.pricePrecision);
    mAvgPriceTableWidgetItem->setValue(avgPrice, operation.pricePrecision);
    mQuantityTableWidgetItem->setValue(operation.quantity);
    mRemainedQuantityTableWidgetItem->setValue(operation.remainedQuantity);
    mPaymentTableWidgetItem->setValue(operation.payment, operation.paymentPrecision);
    mCommissionTableWidgetItem->setValue(operation.commission, operation.commissionPrecision);
    mYieldTableWidgetItem->setValue(operation.yield, operation.yieldPrecision);
    mYieldWithCommissionTableWidgetItem->setValue(operation.yieldWithCommission, operation.yieldWithCommissionPrecision);
    mYieldWithCommissionPercentTableWidgetItem->setValue(
        operation.yieldWithCommissionPercent, avgPrice, operation.pricePrecision
    );
    mRemainedMoneyTableWidgetItem->setValue(quotationToFloat(operation.remainedMoney), operation.remainedMoneyPrecision);
    mTotalMoneyTableWidgetItem->setValue(quotationToFloat(operation.totalMoney), operation.totalMoneyPrecision);
}
