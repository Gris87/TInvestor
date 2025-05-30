#include "src/widgets/treeitems/portfoliotreeitem.h"

#include <QCoreApplication>
#include <QDebug>

#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"



constexpr QChar RUBLE      = QChar(0x20BD);
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
    mDailyYieldPercent(),
    mPricePrecision()
{
    qDebug() << "Create PortfolioTreeItem";

    mInstrumentWidget = instrumentWidgetFactory->newInstance(userStorage, treeWidget());
    treeWidget()->setItemWidget(this, PORTFOLIO_NAME_COLUMN, mInstrumentWidget);

    const QMutexLocker lock(instrumentsStorage->getMutex());

    const Instruments& instruments = instrumentsStorage->getInstruments();
    Instrument         instrument  = instruments[mInstrumentId];

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

const QString& PortfolioTreeItem::getInstrumentId() const
{
    return mInstrumentId;
}

void PortfolioTreeItem::setAvailable(double value)
{
    mAvailable = value;

    if (static_cast<qint64>(mAvailable) * 1000 == static_cast<qint64>(mAvailable * 1000)) // NOLINT(readability-magic-numbers)
    {
        setText(PORTFOLIO_AVAILABLE_COLUMN, QString::number(static_cast<qint64>(mAvailable)));
    }
    else
    {
        setText(PORTFOLIO_AVAILABLE_COLUMN, QString::number(mAvailable, 'f', 2));
    }
}

void PortfolioTreeItem::setPrice(float value)
{
    mPrice = value;

    setText(PORTFOLIO_PRICE_COLUMN, QString::number(mPrice, 'f', mPricePrecision) + " " + RUBLE);
}

void PortfolioTreeItem::setAvgPrice(float value)
{
    mAvgPrice = value;

    setText(PORTFOLIO_AVG_PRICE_COLUMN, QString::number(mAvgPrice, 'f', mPricePrecision) + " " + RUBLE);
}

void PortfolioTreeItem::setCost(double value)
{
    mCost = value;

    setText(PORTFOLIO_COST_COLUMN, QString::number(mCost, 'f', 2) + " " + RUBLE);
}

void PortfolioTreeItem::setPart(float value)
{
    mPart = value;

    setText(PORTFOLIO_PART_COLUMN, QString::number(mPart, 'f', 2) + "%");
}

void PortfolioTreeItem::setYield(float value)
{
    mYield = value;

    const QString prefix = mYield > 0 ? "+" : "";

    setText(PORTFOLIO_YIELD_COLUMN, prefix + QString::number(mYield, 'f', 2) + " " + RUBLE);

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

    setForeground(PORTFOLIO_YIELD_COLUMN, QBrush(color));
}

void PortfolioTreeItem::setYieldPercent(float value, float fromPrice)
{
    mYieldPercent = value;

    const QString prefix = mYieldPercent > 0 ? "+" : "";

    setText(PORTFOLIO_YIELD_PERCENT_COLUMN, prefix + QString::number(mYieldPercent, 'f', 2) + "%");
    setToolTip(PORTFOLIO_YIELD_PERCENT_COLUMN, QObject::tr("From: %1").arg(fromPrice, 0, 'f', mPricePrecision) + " " + RUBLE);

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

    setForeground(PORTFOLIO_YIELD_PERCENT_COLUMN, QBrush(color));
}

void PortfolioTreeItem::setDailyYieldPercent(float value, float fromPrice)
{
    mDailyYieldPercent = value;

    const QString prefix = mDailyYieldPercent > 0 ? "+" : "";

    setText(PORTFOLIO_DAILY_YIELD_COLUMN, prefix + QString::number(mDailyYieldPercent, 'f', 2) + "%");
    setToolTip(PORTFOLIO_DAILY_YIELD_COLUMN, QObject::tr("From: %1").arg(fromPrice, 0, 'f', mPricePrecision) + " " + RUBLE);

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

    setForeground(PORTFOLIO_DAILY_YIELD_COLUMN, QBrush(color));
}

bool PortfolioTreeItem::operator<(const QTreeWidgetItem& another) const
{
    const PortfolioTreeItem* anotherItem = dynamic_cast<const PortfolioTreeItem*>(&another);
    const int                column      = treeWidget()->sortColumn();

    switch (column)
    {
        case PORTFOLIO_NAME_COLUMN:
            return mInstrumentWidget->ticker() < anotherItem->mInstrumentWidget->ticker();
        case PORTFOLIO_AVAILABLE_COLUMN:
            return mAvailable < anotherItem->mAvailable;
        case PORTFOLIO_PRICE_COLUMN:
            return mPrice < anotherItem->mPrice;
        case PORTFOLIO_AVG_PRICE_COLUMN:
            return mAvgPrice < anotherItem->mAvgPrice;
        case PORTFOLIO_COST_COLUMN:
            return mCost < anotherItem->mCost;
        case PORTFOLIO_PART_COLUMN:
            return mPart < anotherItem->mPart;
        case PORTFOLIO_YIELD_COLUMN:
            return mYield < anotherItem->mYield;
        case PORTFOLIO_YIELD_PERCENT_COLUMN:
            return mYieldPercent < anotherItem->mYieldPercent;
        case PORTFOLIO_DAILY_YIELD_COLUMN:
            return mDailyYieldPercent < anotherItem->mDailyYieldPercent;
        default:
            break;
    }

    return false;
}
