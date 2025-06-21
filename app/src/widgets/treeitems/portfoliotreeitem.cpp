#include "src/widgets/treeitems/portfoliotreeitem.h"

#include <QCoreApplication>
#include <QDebug>

#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"



constexpr float ZERO_LIMIT = 0.0001f;

const QColor GREEN_COLOR  = QColor("#2BD793"); // clazy:exclude=non-pod-global-static
const QColor RED_COLOR    = QColor("#ED6F7E"); // clazy:exclude=non-pod-global-static
const QColor NORMAL_COLOR = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



PortfolioTreeItem::PortfolioTreeItem(
    QTreeWidgetItem*          parent,
    IInstrumentWidgetFactory* instrumentWidgetFactory,
    IUserStorage*             userStorage,
    IInstrumentsStorage*      instrumentsStorage,
    const QString&            instrumentId,
    int                       type
) :
    QTreeWidgetItem(parent, type),
    mInstrumentWidget(),
    mInstrumentId(instrumentId),
    mAvailable(),
    mPrice(),
    mAvgPrice(),
    mCost(),
    mPart(),
    mYield(),
    mYieldPercent(),
    mDailyYield(),
    mCostForDailyYield(),
    mPriceForDailyYield(),
    mDailyYieldPercent(),
    mPricePrecision()
{
    qDebug() << "Create PortfolioTreeItem";

    mInstrumentWidget = instrumentWidgetFactory->newInstance(userStorage, treeWidget());
    treeWidget()->setItemWidget(this, PORTFOLIO_OLD_NAME_COLUMN, mInstrumentWidget);

    instrumentsStorage->lock();
    const Instruments& instruments = instrumentsStorage->getInstruments();
    Instrument         instrument  = instruments[mInstrumentId];
    instrumentsStorage->unlock();

    const QIcon instrumentLogo(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), mInstrumentId));

    if (instrument.ticker == "" || instrument.name == "")
    {
        instrument.ticker         = mInstrumentId;
        instrument.name           = "?????";
        instrument.pricePrecision = 2;
    }

    mPricePrecision = instrument.pricePrecision;

    mInstrumentWidget->setInstrumentLogo(instrumentLogo);
    mInstrumentWidget->setTicker(instrument.ticker);
    mInstrumentWidget->setFullText(instrument.name);
}

PortfolioTreeItem::~PortfolioTreeItem()
{
    qDebug() << "Destroy PortfolioTreeItem";
}

void PortfolioTreeItem::setAvailable(double value)
{
    mAvailable = value;

    if (static_cast<qint64>(mAvailable) * 1000 == static_cast<qint64>(mAvailable * 1000)) // NOLINT(readability-magic-numbers)
    {
        setText(PORTFOLIO_OLD_AVAILABLE_COLUMN, QString::number(static_cast<qint64>(mAvailable)));
    }
    else
    {
        setText(PORTFOLIO_OLD_AVAILABLE_COLUMN, QString::number(mAvailable, 'f', 2));
    }
}

void PortfolioTreeItem::setPrice(float value)
{
    mPrice = value;

    setText(PORTFOLIO_OLD_PRICE_COLUMN, QString::number(mPrice, 'f', mPricePrecision) + " \u20BD");
}

void PortfolioTreeItem::setAvgPrice(float valueFifo, float valueWavg)
{
    mAvgPrice = valueFifo;

    setText(PORTFOLIO_OLD_AVG_PRICE_COLUMN, QString::number(mAvgPrice, 'f', mPricePrecision) + " \u20BD");
    setToolTip(
        PORTFOLIO_OLD_AVG_PRICE_COLUMN,
        QObject::tr("Average price by WAVG: %1").arg(valueWavg, 0, 'f', mPricePrecision) + " \u20BD"
    );
}

void PortfolioTreeItem::setCost(double value)
{
    mCost = value;

    setText(PORTFOLIO_OLD_COST_COLUMN, QString::number(mCost, 'f', 2) + " \u20BD");
}

void PortfolioTreeItem::setPart(float value)
{
    mPart = value;

    setText(PORTFOLIO_OLD_PART_COLUMN, QString::number(mPart, 'f', 2) + "%");
}

void PortfolioTreeItem::setYield(float value)
{
    mYield = value;

    const QString prefix = mYield > 0 ? "+" : "";

    setText(PORTFOLIO_OLD_YIELD_COLUMN, prefix + QString::number(mYield, 'f', 2) + " \u20BD");

    QColor color;

    if (mYield > -ZERO_LIMIT && mYield < ZERO_LIMIT)
    {
        color = NORMAL_COLOR;
    }
    else
    {
        if (mYield > 0)
        {
            color = GREEN_COLOR;
        }
        else
        {
            color = RED_COLOR;
        }
    }

    setForeground(PORTFOLIO_OLD_YIELD_COLUMN, QBrush(color));
}

void PortfolioTreeItem::setYieldPercent(float value, float fromPrice)
{
    mYieldPercent = value;

    const QString prefix = mYieldPercent > 0 ? "+" : "";

    setText(PORTFOLIO_OLD_YIELD_PERCENT_COLUMN, prefix + QString::number(mYieldPercent, 'f', 2) + "%");
    setToolTip(PORTFOLIO_OLD_YIELD_PERCENT_COLUMN, QObject::tr("From: %1").arg(fromPrice, 0, 'f', mPricePrecision) + " \u20BD");

    QColor color;

    if (mYieldPercent > -ZERO_LIMIT && mYieldPercent < ZERO_LIMIT)
    {
        color = NORMAL_COLOR;
    }
    else
    {
        if (mYieldPercent > 0)
        {
            color = GREEN_COLOR;
        }
        else
        {
            color = RED_COLOR;
        }
    }

    setForeground(PORTFOLIO_OLD_YIELD_PERCENT_COLUMN, QBrush(color));
}

void PortfolioTreeItem::setDailyYield(float value)
{
    mDailyYield = value;
}

void PortfolioTreeItem::setCostForDailyYield(double value)
{
    mCostForDailyYield = value;
}

void PortfolioTreeItem::setDailyYieldPercent(float value, float fromPrice)
{
    mDailyYieldPercent  = value;
    mPriceForDailyYield = fromPrice;

    const QString prefix = mDailyYieldPercent > 0 ? "+" : "";

    setText(PORTFOLIO_OLD_DAILY_YIELD_COLUMN, prefix + QString::number(mDailyYieldPercent, 'f', 2) + "%");
    setToolTip(PORTFOLIO_OLD_DAILY_YIELD_COLUMN, QObject::tr("From: %1").arg(fromPrice, 0, 'f', mPricePrecision) + " \u20BD");

    QColor color;

    if (mDailyYieldPercent > -ZERO_LIMIT && mDailyYieldPercent < ZERO_LIMIT)
    {
        color = NORMAL_COLOR;
    }
    else
    {
        if (mDailyYieldPercent > 0)
        {
            color = GREEN_COLOR;
        }
        else
        {
            color = RED_COLOR;
        }
    }

    setForeground(PORTFOLIO_OLD_DAILY_YIELD_COLUMN, QBrush(color));
}

const QString& PortfolioTreeItem::instrumentId() const
{
    return mInstrumentId;
}

QString PortfolioTreeItem::name() const
{
    return mInstrumentWidget->fullText();
}

double PortfolioTreeItem::available() const
{
    return mAvailable;
}

float PortfolioTreeItem::price() const
{
    return mPrice;
}

float PortfolioTreeItem::avgPrice() const
{
    return mAvgPrice;
}

double PortfolioTreeItem::cost() const
{
    return mCost;
}

float PortfolioTreeItem::part() const
{
    return mPart;
}

float PortfolioTreeItem::yield() const
{
    return mYield;
}

float PortfolioTreeItem::yieldPercent() const
{
    return mYieldPercent;
}

float PortfolioTreeItem::dailyYield() const
{
    return mDailyYield;
}

float PortfolioTreeItem::priceForDailyYield() const
{
    return mPriceForDailyYield;
}

double PortfolioTreeItem::costForDailyYield() const
{
    return mCostForDailyYield;
}

float PortfolioTreeItem::dailyYieldPercent() const
{
    return mDailyYieldPercent;
}

qint8 PortfolioTreeItem::pricePrecision() const
{
    return mPricePrecision;
}

bool PortfolioTreeItem::operator<(const QTreeWidgetItem& another) const
{
    const PortfolioTreeItem* anotherItem = dynamic_cast<const PortfolioTreeItem*>(&another);
    const int                column      = treeWidget()->sortColumn();

    switch (column)
    {
        case PORTFOLIO_OLD_NAME_COLUMN:
            return mInstrumentWidget->ticker() < anotherItem->mInstrumentWidget->ticker();
        case PORTFOLIO_OLD_AVAILABLE_COLUMN:
            return mAvailable < anotherItem->mAvailable;
        case PORTFOLIO_OLD_PRICE_COLUMN:
            return mPrice < anotherItem->mPrice;
        case PORTFOLIO_OLD_AVG_PRICE_COLUMN:
            return mAvgPrice < anotherItem->mAvgPrice;
        case PORTFOLIO_OLD_COST_COLUMN:
            return mCost < anotherItem->mCost;
        case PORTFOLIO_OLD_PART_COLUMN:
            return mPart < anotherItem->mPart;
        case PORTFOLIO_OLD_YIELD_COLUMN:
            return mYield < anotherItem->mYield;
        case PORTFOLIO_OLD_YIELD_PERCENT_COLUMN:
            return mYieldPercent < anotherItem->mYieldPercent;
        case PORTFOLIO_OLD_DAILY_YIELD_COLUMN:
            return mDailyYieldPercent < anotherItem->mDailyYieldPercent;
        default:
            break;
    }

    return false;
}
