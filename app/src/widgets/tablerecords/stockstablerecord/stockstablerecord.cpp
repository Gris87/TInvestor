#include "src/widgets/tablerecords/stockstablerecord/stockstablerecord.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPushButton>

#include "src/grpc/utils.h"



const QColor CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr float HUNDRED_PERCENT = 100.0f;



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
    mPrecision = quotationPrecision(mStock->meta.minPriceIncrement);
    mStock->mutex->unlock();

    mInstrumentTableItemWidget =
        instrumentTableItemWidgetFactory->newInstance(userStorage, tableWidget); // tableWidget will take ownership

    IActionsTableItemWidget* actionsTableItemWidget = actionsTableItemWidgetFactory->newInstance(
        orderWavesDialogFactory, orderWavesWidgetFactory, orderBookThread, httpClient, mStock, mPrecision, tableWidget
    ); // tableWidget will take ownership

    const int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    // clang-format off
    tableWidget->setCellWidget(rowIndex, STOCKS_STOCK_COLUMN,       mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex,       STOCKS_STOCK_COLUMN,       mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex,       STOCKS_PRICE_COLUMN,       mPriceTableWidgetItem);
    tableWidget->setItem(rowIndex,       STOCKS_DAY_CHANGE_COLUMN,  mDayChangeTableWidgetItem);
    tableWidget->setItem(rowIndex,       STOCKS_DATE_CHANGE_COLUMN, mDateChangeTableWidgetItem);
    tableWidget->setItem(rowIndex,       STOCKS_TURNOVER_COLUMN,    mTurnoverTableWidgetItem);
    tableWidget->setItem(rowIndex,       STOCKS_PAYBACK_COLUMN,     mPaybackTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, STOCKS_ACTIONS_COLUMN,     actionsTableItemWidget);
    // clang-format on
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
                                 ? ((price / mStock->operational.dayStartPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                                 : 0;
    const float dateChange = mStock->operational.specifiedDatePrice > 0
                                 ? ((price / mStock->operational.specifiedDatePrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT
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
    const int row = mInstrumentTableItemWidget->row();

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

void StocksTableRecord::exportToExcel(QXlsx::Document& doc)
{
    int row = mInstrumentTableItemWidget->row() + 2; // Header and start index from 1

    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    // clang-format off
    doc.write(row, STOCKS_STOCK_COLUMN + 1,       mInstrumentTableItemWidget->fullText(), cellStyle);
    doc.write(row, STOCKS_STOCK_COLUMN + 2,       mInstrumentTableItemWidget->forQualInvestorFlag(), cellStyle);
    doc.write(row, STOCKS_PRICE_COLUMN + 2,       mPriceTableWidgetItem->getValue(), createRubleFormat(CELL_FONT_COLOR, mPriceTableWidgetItem->getPrecision()));
    doc.write(row, STOCKS_DAY_CHANGE_COLUMN + 2,  mDayChangeTableWidgetItem->getValue() / HUNDRED_PERCENT, createPercentFormat(mDayChangeTableWidgetItem->foreground().color(), true));
    doc.write(row, STOCKS_DATE_CHANGE_COLUMN + 2, mDateChangeTableWidgetItem->getValue() / HUNDRED_PERCENT, createPercentFormat(mDateChangeTableWidgetItem->foreground().color(), true));
    doc.write(row, STOCKS_TURNOVER_COLUMN + 2,    mTurnoverTableWidgetItem->getValue(), createRubleFormat(mTurnoverTableWidgetItem->foreground().color(), 0));
    doc.write(row, STOCKS_PAYBACK_COLUMN + 2,     mPaybackTableWidgetItem->getValue() / HUNDRED_PERCENT, createPercentFormat(mPaybackTableWidgetItem->foreground().color(), false));
    // clang-format on
}

QXlsx::Format StocksTableRecord::createRubleFormat(const QColor& color, int precision) const
{
    QXlsx::Format res;

    if (precision > 0)
    {
        res.setNumberFormat(QString("0.%1 \u20BD").arg("", precision, '0'));
    }
    else
    {
        res.setNumberFormat("0 \u20BD");
    }

    res.setFillPattern(QXlsx::Format::PatternSolid);
    res.setBorderStyle(QXlsx::Format::BorderThin);
    res.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    res.setFontColor(color);

    return res;
}

QXlsx::Format StocksTableRecord::createPercentFormat(const QColor& color, bool withPlus) const
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
