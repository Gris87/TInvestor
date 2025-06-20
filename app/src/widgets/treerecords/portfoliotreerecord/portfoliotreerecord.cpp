#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"

#include <QDebug>



const QColor CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static

constexpr float HUNDRED_PERCENT = 100.0f;



PortfolioTreeRecord::PortfolioTreeRecord(
    IInstrumentWidgetFactory* instrumentWidgetFactory,
    IUserStorage*             userStorage,
    IInstrumentsStorage*      instrumentsStorage,
    CategoryTreeItem*         categoryTreeItem,
    const QString&            instrumentId,
    QObject*                  parent
) :
    IPortfolioTreeRecord(parent),
    mPortfolioTreeItem(
        new PortfolioTreeItem(categoryTreeItem, instrumentWidgetFactory, userStorage, instrumentsStorage, instrumentId)
    )
{
    qDebug() << "Create PortfolioTreeRecord";
}

PortfolioTreeRecord::~PortfolioTreeRecord()
{
    qDebug() << "Destroy PortfolioTreeRecord";
}

void PortfolioTreeRecord::setPortfolioItem(const PortfolioItem& item)
{
    if (item.showPrices)
    {
        mPortfolioTreeItem->setPrice(item.price);
        mPortfolioTreeItem->setAvgPrice(item.avgPriceFifo, item.avgPriceWavg);
        mPortfolioTreeItem->setYield(item.yield);
        mPortfolioTreeItem->setYieldPercent(item.yieldPercent, item.avgPriceFifo);
        mPortfolioTreeItem->setDailyYield(item.dailyYield);
        mPortfolioTreeItem->setCostForDailyYield(item.costForDailyYield);
        mPortfolioTreeItem->setDailyYieldPercent(item.dailyYieldPercent, item.priceForDailyYield);
    }

    mPortfolioTreeItem->setAvailable(item.available);
    mPortfolioTreeItem->setCost(item.cost);
    mPortfolioTreeItem->setPart(item.part);
}

void PortfolioTreeRecord::updatePrice(float price)
{
    const double currentCost       = mPortfolioTreeItem->available() * price;
    const float  yield             = currentCost - mPortfolioTreeItem->cost();
    const float  yieldPercent      = (yield / mPortfolioTreeItem->cost()) * HUNDRED_PERCENT;
    const float  dailyYield        = currentCost - mPortfolioTreeItem->costForDailyYield();
    const float  dailyYieldPercent = (dailyYield / mPortfolioTreeItem->costForDailyYield()) * HUNDRED_PERCENT;

    mPortfolioTreeItem->setPrice(price);
    mPortfolioTreeItem->setYield(yield);
    mPortfolioTreeItem->setYieldPercent(yieldPercent, mPortfolioTreeItem->avgPrice());
    mPortfolioTreeItem->setDailyYield(dailyYield);
    mPortfolioTreeItem->setDailyYieldPercent(dailyYieldPercent, mPortfolioTreeItem->priceForDailyYield());
}

void PortfolioTreeRecord::exportToExcel(QXlsx::Document& doc, int row) const
{
    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    if (mPortfolioTreeItem->text(PORTFOLIO_OLD_PRICE_COLUMN) != "")
    {
        // clang-format off
        doc.write(row, PORTFOLIO_OLD_NAME_COLUMN + 1,          mPortfolioTreeItem->name(),                                cellStyle);
        doc.write(row, PORTFOLIO_OLD_AVAILABLE_COLUMN + 1,     mPortfolioTreeItem->available(),                           cellStyle);
        doc.write(row, PORTFOLIO_OLD_PRICE_COLUMN + 1,         mPortfolioTreeItem->price(),                               createRubleFormat(CELL_FONT_COLOR, false, mPortfolioTreeItem->pricePrecision()));
        doc.write(row, PORTFOLIO_OLD_AVG_PRICE_COLUMN + 1,     mPortfolioTreeItem->avgPrice(),                            createRubleFormat(CELL_FONT_COLOR, false, mPortfolioTreeItem->pricePrecision()));
        doc.write(row, PORTFOLIO_OLD_COST_COLUMN + 1,          mPortfolioTreeItem->cost(),                                createRubleFormat(CELL_FONT_COLOR, false, 2));
        doc.write(row, PORTFOLIO_OLD_PART_COLUMN + 1,          mPortfolioTreeItem->part() / HUNDRED_PERCENT,              createPercentFormat(CELL_FONT_COLOR, false));
        doc.write(row, PORTFOLIO_OLD_YIELD_COLUMN + 1,         mPortfolioTreeItem->yield(),                               createRubleFormat(mPortfolioTreeItem->foreground(PORTFOLIO_OLD_YIELD_COLUMN).color(), true, 2));
        doc.write(row, PORTFOLIO_OLD_YIELD_PERCENT_COLUMN + 1, mPortfolioTreeItem->yieldPercent() / HUNDRED_PERCENT,      createPercentFormat(mPortfolioTreeItem->foreground(PORTFOLIO_OLD_YIELD_PERCENT_COLUMN).color(), true));
        doc.write(row, PORTFOLIO_OLD_DAILY_YIELD_COLUMN + 1,   mPortfolioTreeItem->dailyYieldPercent() / HUNDRED_PERCENT, createPercentFormat(mPortfolioTreeItem->foreground(PORTFOLIO_OLD_DAILY_YIELD_COLUMN).color(), true));
        // clang-format on
    }
    else
    {
        // clang-format off
        doc.write(row, PORTFOLIO_OLD_NAME_COLUMN + 1,          mPortfolioTreeItem->name(),                   cellStyle);
        doc.write(row, PORTFOLIO_OLD_AVAILABLE_COLUMN + 1,     mPortfolioTreeItem->available(),              cellStyle);
        doc.write(row, PORTFOLIO_OLD_PRICE_COLUMN + 1,         "",                                           cellStyle);
        doc.write(row, PORTFOLIO_OLD_AVG_PRICE_COLUMN + 1,     "",                                           cellStyle);
        doc.write(row, PORTFOLIO_OLD_COST_COLUMN + 1,          mPortfolioTreeItem->cost(),                   createRubleFormat(CELL_FONT_COLOR, false, 2));
        doc.write(row, PORTFOLIO_OLD_PART_COLUMN + 1,          mPortfolioTreeItem->part() / HUNDRED_PERCENT, createPercentFormat(CELL_FONT_COLOR, false));
        doc.write(row, PORTFOLIO_OLD_YIELD_COLUMN + 1,         "",                                           cellStyle);
        doc.write(row, PORTFOLIO_OLD_YIELD_PERCENT_COLUMN + 1, "",                                           cellStyle);
        doc.write(row, PORTFOLIO_OLD_DAILY_YIELD_COLUMN + 1,   "",                                           cellStyle);
        // clang-format on
    }
}

QXlsx::Format PortfolioTreeRecord::createRubleFormat(const QColor& color, bool withPlus, int precision) const
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

QXlsx::Format PortfolioTreeRecord::createPercentFormat(const QColor& color, bool withPlus) const
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

float PortfolioTreeRecord::yield() const
{
    return mPortfolioTreeItem->yield();
}

float PortfolioTreeRecord::dailyYield() const
{
    return mPortfolioTreeItem->dailyYield();
}
