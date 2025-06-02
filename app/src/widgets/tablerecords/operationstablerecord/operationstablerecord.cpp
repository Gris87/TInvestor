#include "src/widgets/tablerecords/operationstablerecord/operationstablerecord.h"

#include <QCoreApplication>
#include <QDebug>

#include "src/grpc/utils.h"



const char* const DATETIME_FORMAT       = "yyyy-MM-dd hh:mm:ss";
const QColor      CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr float HUNDRED_PERCENT = 100.0f;



OperationsTableRecord::OperationsTableRecord(
    QTableWidget*                      tableWidget,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    QObject*                           parent
) :
    IOperationsTableRecord(parent),
    mInstrumentsStorage(instrumentsStorage),
    mTimeTableWidgetItem(new TimeTableItem()),
    mInstrumentTableItemWidget(),
    mDescriptionTableWidgetItem(new QTableWidgetItem()),
    mPriceTableWidgetItem(new MoneyTableItem()),
    mAvgPriceFifoTableWidgetItem(new MoneyTableItem()),
    mAvgPriceWavgTableWidgetItem(new MoneyTableItem()),
    mQuantityTableWidgetItem(new IntegerTableItem()),
    mRemainedQuantityTableWidgetItem(new IntegerTableItem()),
    mPaymentTableWidgetItem(new MoneyChangeTableItem()),
    mCommissionTableWidgetItem(new MoneyChangeTableItem()),
    mYieldTableWidgetItem(new MoneyChangeTableItem()),
    mYieldWithCommissionTableWidgetItem(new MoneyChangeTableItem()),
    mYieldWithCommissionPercentTableWidgetItem(new MoneyChangePercentTableItem()),
    mTotalYieldWithCommissionTableWidgetItem(new MoneyChangeTableItem()),
    mTotalYieldWithCommissionPercentTableWidgetItem(new MoneyChangePercentTableItem()),
    mRemainedMoneyTableWidgetItem(new MoneyTableItem()),
    mTotalMoneyTableWidgetItem(new MoneyTableItem())
{
    qDebug() << "Create OperationsTableRecord";

    mInstrumentTableItemWidget =
        instrumentTableItemWidgetFactory->newInstance(userStorage, tableWidget); // tableWidget will take ownership

    const int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    // clang-format off
    tableWidget->setItem(rowIndex,       OPERATIONS_TIME_COLUMN,                                mTimeTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, OPERATIONS_NAME_COLUMN,                                mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex,       OPERATIONS_NAME_COLUMN,                                mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex,       OPERATIONS_DESCRIPTION_COLUMN,                         mDescriptionTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_PRICE_COLUMN,                               mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_AVG_PRICE_FIFO_COLUMN,                      mAvgPriceFifoTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_AVG_PRICE_WAVG_COLUMN,                      mAvgPriceWavgTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_QUANTITY_COLUMN,                            mQuantityTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_REMAINED_QUANTITY_COLUMN,                   mRemainedQuantityTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_PAYMENT_COLUMN,                             mPaymentTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_COMMISSION_COLUMN,                          mCommissionTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_YIELD_COLUMN,                               mYieldTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_YIELD_WITH_COMMISSION_COLUMN,               mYieldWithCommissionTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN,       mYieldWithCommissionPercentTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN,         mTotalYieldWithCommissionTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN, mTotalYieldWithCommissionPercentTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_REMAINED_MONEY_COLUMN,                      mRemainedMoneyTableWidgetItem);
    tableWidget->setItem(rowIndex,       OPERATIONS_TOTAL_MONEY_COLUMN,                         mTotalMoneyTableWidgetItem);
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

    if (instrument.ticker == "" || instrument.name == "")
    {
        instrument.ticker         = operation.instrumentId;
        instrument.name           = "?????";
        instrument.pricePrecision = 2;
    }

    mTimeTableWidgetItem->setValue(QDateTime::fromMSecsSinceEpoch(operation.timestamp));
    mInstrumentTableItemWidget->setInstrumentLogo(instrumentLogo);
    mInstrumentTableItemWidget->setTicker(instrument.ticker);
    mInstrumentTableItemWidget->setFullText(instrument.name);
    mDescriptionTableWidgetItem->setText(operation.description);
    mPriceTableWidgetItem->setValue(operation.price, instrument.pricePrecision);
    mAvgPriceFifoTableWidgetItem->setValue(operation.avgPriceFifo, instrument.pricePrecision);
    mAvgPriceWavgTableWidgetItem->setValue(operation.avgPriceWavg, instrument.pricePrecision);
    mQuantityTableWidgetItem->setValue(operation.quantity);
    mRemainedQuantityTableWidgetItem->setValue(operation.remainedQuantity);
    mPaymentTableWidgetItem->setValue(operation.payment, false, operation.paymentPrecision);
    mCommissionTableWidgetItem->setValue(operation.commission, false, operation.commissionPrecision);
    mYieldTableWidgetItem->setValue(operation.yield, false, 2);
    mYieldWithCommissionTableWidgetItem->setValue(operation.yieldWithCommission, true, 2);
    mYieldWithCommissionPercentTableWidgetItem->setValue(
        operation.yieldWithCommissionPercent, operation.avgCost, instrument.pricePrecision
    );
    mTotalYieldWithCommissionTableWidgetItem->setValue(quotationToFloat(operation.totalYieldWithCommission), true, 2);
    mTotalYieldWithCommissionPercentTableWidgetItem->setValue(
        operation.totalYieldWithCommissionPercent, quotationToFloat(operation.maxInputMoney), 2
    );
    mRemainedMoneyTableWidgetItem->setValue(quotationToFloat(operation.remainedMoney), 2);
    mTotalMoneyTableWidgetItem->setValue(quotationToFloat(operation.totalMoney), 2);
}

void OperationsTableRecord::exportToExcel(QXlsx::Document& doc) const
{
    const int row = mTimeTableWidgetItem->row() + 2; // Header and start index from 1

    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    QXlsx::Format dateFormat;
    dateFormat.setNumberFormat(DATETIME_FORMAT);
    dateFormat.setFillPattern(QXlsx::Format::PatternSolid);
    dateFormat.setBorderStyle(QXlsx::Format::BorderThin);
    dateFormat.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    dateFormat.setFontColor(CELL_FONT_COLOR);

    // clang-format off
    doc.write(row, OPERATIONS_TIME_COLUMN + 1,                                mTimeTableWidgetItem->value(), dateFormat);
    doc.write(row, OPERATIONS_NAME_COLUMN + 1,                                mInstrumentTableItemWidget->fullText(), cellStyle);
    doc.write(row, OPERATIONS_DESCRIPTION_COLUMN + 1,                         mDescriptionTableWidgetItem->text(), cellStyle);
    doc.write(row, OPERATIONS_PRICE_COLUMN + 1,                               mPriceTableWidgetItem->value(), createRubleFormat(CELL_FONT_COLOR, false, mPriceTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_AVG_PRICE_FIFO_COLUMN + 1,                      mAvgPriceFifoTableWidgetItem->value(), createRubleFormat(CELL_FONT_COLOR, false, mAvgPriceFifoTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_AVG_PRICE_WAVG_COLUMN + 1,                      mAvgPriceWavgTableWidgetItem->value(), createRubleFormat(CELL_FONT_COLOR, false, mAvgPriceWavgTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_QUANTITY_COLUMN + 1,                            mQuantityTableWidgetItem->value(), cellStyle);
    doc.write(row, OPERATIONS_REMAINED_QUANTITY_COLUMN + 1,                   mRemainedQuantityTableWidgetItem->value(), cellStyle);
    doc.write(row, OPERATIONS_PAYMENT_COLUMN + 1,                             mPaymentTableWidgetItem->value(), createRubleFormat(CELL_FONT_COLOR, true, mPaymentTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_COMMISSION_COLUMN + 1,                          mCommissionTableWidgetItem->value(), createRubleFormat(CELL_FONT_COLOR, true, mCommissionTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_YIELD_COLUMN + 1,                               mYieldTableWidgetItem->value(), createRubleFormat(CELL_FONT_COLOR, true, mYieldTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_YIELD_WITH_COMMISSION_COLUMN + 1,               mYieldWithCommissionTableWidgetItem->value(), createRubleFormat(mYieldWithCommissionTableWidgetItem->foreground().color(), true, mYieldWithCommissionTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_YIELD_WITH_COMMISSION_PERCENT_COLUMN + 1,       mYieldWithCommissionPercentTableWidgetItem->value() / HUNDRED_PERCENT, createPercentFormat(mYieldWithCommissionPercentTableWidgetItem->foreground().color(), true));
    doc.write(row, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_COLUMN + 1,         mTotalYieldWithCommissionTableWidgetItem->value(), createRubleFormat(mTotalYieldWithCommissionTableWidgetItem->foreground().color(), true, mTotalYieldWithCommissionTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_TOTAL_YIELD_WITH_COMMISSION_PERCENT_COLUMN + 1, mTotalYieldWithCommissionPercentTableWidgetItem->value() / HUNDRED_PERCENT, createPercentFormat(mTotalYieldWithCommissionPercentTableWidgetItem->foreground().color(), true));
    doc.write(row, OPERATIONS_REMAINED_MONEY_COLUMN + 1,                      mRemainedMoneyTableWidgetItem->value(), createRubleFormat(CELL_FONT_COLOR, false, mRemainedMoneyTableWidgetItem->precision()));
    doc.write(row, OPERATIONS_TOTAL_MONEY_COLUMN + 1,                         mTotalMoneyTableWidgetItem->value(), createRubleFormat(CELL_FONT_COLOR, false, mTotalMoneyTableWidgetItem->precision()));
    // clang-format on
}

QXlsx::Format OperationsTableRecord::createRubleFormat(const QColor& color, bool withPlus, int precision) const
{
    QXlsx::Format res;

    if (withPlus)
    {
        res.setNumberFormat(QString("+0.%1 \u20BD;-0.%1 \u20BD;0.%1 \u20BD").arg("", precision, '0'));
    }
    else
    {
        res.setNumberFormat(QString("0.%1 \u20BD").arg("", precision, '0'));
    }

    res.setFillPattern(QXlsx::Format::PatternSolid);
    res.setBorderStyle(QXlsx::Format::BorderThin);
    res.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    res.setFontColor(color);

    return res;
}

QXlsx::Format OperationsTableRecord::createPercentFormat(const QColor& color, bool withPlus) const
{
    QXlsx::Format res;

    if (withPlus)
    {
        res.setNumberFormat("+0.00%;-0.00%;0.00%");
    }
    else
    {
        res.setNumberFormat("0.00%");
    }

    res.setFillPattern(QXlsx::Format::PatternSolid);
    res.setBorderStyle(QXlsx::Format::BorderThin);
    res.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    res.setFontColor(color);

    return res;
}
