#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodel.h"

#include <QBrush>
#include <QDebug>
#include <algorithm>
#include <execution>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/tablemodels/modelroles.h"
#include "src/widgets/tablemodels/portfoliotreemodel/comparators.h"



constexpr float ZERO_LIMIT      = 0.0001f;
constexpr float HUNDRED_PERCENT = 100.0f;

const QBrush GREEN_COLOR               = QBrush(QColor("#2BD793")); // clazy:exclude=non-pod-global-static
const QBrush RED_COLOR                 = QBrush(QColor("#ED6F7E")); // clazy:exclude=non-pod-global-static
const QBrush NORMAL_COLOR              = QBrush(QColor("#97AEC4")); // clazy:exclude=non-pod-global-static
const QColor CATEGORY_BACKGROUND_COLOR = QColor("#354450");         // clazy:exclude=non-pod-global-static
const QColor CATEGORY_FONT_COLOR       = QColor("#BFBFBF");         // clazy:exclude=non-pod-global-static
const QColor CELL_BACKGROUND_COLOR     = QColor("#2C3C4B");         // clazy:exclude=non-pod-global-static
const QColor CELL_FONT_COLOR           = QColor("#97AEC4");         // clazy:exclude=non-pod-global-static



PortfolioTreeModel::PortfolioTreeModel(QObject* parent) :
    IPortfolioTreeModel(parent),
    mHeader(),
    mHelpIcon(":/assets/images/question.png"),
    mPortfolio(),
    mSortColumn(PORTFOLIO_NAME_COLUMN),
    mSortOrder(Qt::AscendingOrder),
    mLastPricesUpdates()
{
    qDebug() << "Create PortfolioTreeModel";

    mHeader << tr("Name") << tr("Available") << tr("Price") << tr("Avg price") << tr("Cost") << tr("Part") << tr("Yield")
            << tr("Yield, %") << tr("Daily yield, %");
    Q_ASSERT_X(mHeader.size() == PORTFOLIO_COLUMN_COUNT, __FUNCTION__, "Header is incorrect");
}

PortfolioTreeModel::~PortfolioTreeModel()
{
    qDebug() << "Destroy PortfolioTreeModel";
}

QModelIndex PortfolioTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        return createIndex(row, column);
    }

    return createIndex(row, column, &mPortfolio.positionsList.at(parent.row()));
}

QModelIndex PortfolioTreeModel::parent(const QModelIndex& child) const
{
    PortfolioCategoryItem* category = reinterpret_cast<PortfolioCategoryItem*>(child.internalPointer());

    if (category != nullptr)
    {
        return createIndex(category->id, 0);
    }

    return QModelIndex();
}

int PortfolioTreeModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        return mPortfolio.positionsList.size();
    }

    PortfolioCategoryItem* category = reinterpret_cast<PortfolioCategoryItem*>(parent.internalPointer());

    if (category == nullptr)
    {
        return mPortfolio.positionsList.at(parent.row()).items.size();
    }

    return 0;
}

int PortfolioTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return PORTFOLIO_COLUMN_COUNT;
}

QVariant PortfolioTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return mHeader.at(section);
        }

        return section + 1;
    }

    if (role == Qt::ToolTipRole)
    {
        if (section == PORTFOLIO_AVG_PRICE_COLUMN)
        {
            return tr("Average price by FIFO");
        }

        return QVariant();
    }

    if (role == Qt::DecorationRole)
    {
        if (section == PORTFOLIO_AVG_PRICE_COLUMN)
        {
            return mHelpIcon;
        }

        return QVariant();
    }

    return QVariant();
}

static QVariant categoryNameDisplayRole(const PortfolioCategoryItem& category)
{
    return category.name;
}

static QVariant categoryCostDisplayRole(const PortfolioCategoryItem& category)
{
    return QString::number(category.cost, 'f', 2) + " \u20BD";
}

static QVariant categoryPartDisplayRole(const PortfolioCategoryItem& category)
{
    return QString::number(category.part, 'f', 2) + "%";
}

static QVariant categoryNothingDisplayRole(const PortfolioCategoryItem& /*category*/)
{
    return QVariant();
}

using CategoryDisplayRoleHandler = QVariant (*)(const PortfolioCategoryItem& category);

static const CategoryDisplayRoleHandler CATEGORY_DISPLAY_ROLE_HANDLER[PORTFOLIO_COLUMN_COUNT]{
    categoryNameDisplayRole,
    categoryNothingDisplayRole,
    categoryNothingDisplayRole,
    categoryNothingDisplayRole,
    categoryCostDisplayRole,
    categoryPartDisplayRole,
    categoryNothingDisplayRole,
    categoryNothingDisplayRole,
    categoryNothingDisplayRole
};

static QVariant itemNameDisplayRole(const PortfolioItem& item)
{
    return item.instrumentTicker;
}

static QVariant itemAvailableDisplayRole(const PortfolioItem& item)
{
    double value = item.available;

    if (static_cast<qint64>(value) * 1000 == static_cast<qint64>(value * 1000)) // NOLINT(readability-magic-numbers)
    {
        return QString::number(static_cast<qint64>(value));
    }
    else
    {
        return QString::number(value, 'f', 2);
    }
}

static QVariant itemPriceDisplayRole(const PortfolioItem& item)
{
    if (!item.showPrices)
    {
        return QVariant();
    }

    return QString::number(item.price, 'f', item.pricePrecision) + " \u20BD";
}

static QVariant itemAvgPriceDisplayRole(const PortfolioItem& item)
{
    if (!item.showPrices)
    {
        return QVariant();
    }

    return QString::number(item.avgPriceFifo, 'f', item.pricePrecision) + " \u20BD";
}

static QVariant itemCostDisplayRole(const PortfolioItem& item)
{
    return QString::number(item.cost, 'f', 2) + " \u20BD";
}

static QVariant itemPartDisplayRole(const PortfolioItem& item)
{
    return QString::number(item.part, 'f', 2) + "%";
}

static QVariant itemYieldDisplayRole(const PortfolioItem& item)
{
    if (!item.showPrices)
    {
        return QVariant();
    }

    const QString prefix = item.yield > 0 ? "+" : "";

    return prefix + QString::number(item.yield, 'f', 2) + " \u20BD";
}

static QVariant itemYieldPercentDisplayRole(const PortfolioItem& item)
{
    if (!item.showPrices)
    {
        return QVariant();
    }

    const QString prefix = item.yieldPercent > 0 ? "+" : "";

    return prefix + QString::number(item.yieldPercent, 'f', 2) + "%";
}

static QVariant itemDailyYieldDisplayRole(const PortfolioItem& item)
{
    if (!item.showPrices)
    {
        return QVariant();
    }

    const QString prefix = item.dailyYieldPercent > 0 ? "+" : "";

    return prefix + QString::number(item.dailyYieldPercent, 'f', 2) + "%";
}

using ItemDisplayRoleHandler = QVariant (*)(const PortfolioItem& item);

static const ItemDisplayRoleHandler ITEM_DISPLAY_ROLE_HANDLER[PORTFOLIO_COLUMN_COUNT]{
    itemNameDisplayRole,
    itemAvailableDisplayRole,
    itemPriceDisplayRole,
    itemAvgPriceDisplayRole,
    itemCostDisplayRole,
    itemPartDisplayRole,
    itemYieldDisplayRole,
    itemYieldPercentDisplayRole,
    itemDailyYieldDisplayRole
};

static QVariant itemYieldForegroundRole(const PortfolioItem& item)
{
    if (item.yield > -ZERO_LIMIT && item.yield < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (item.yield > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant itemYieldPercentForegroundRole(const PortfolioItem& item)
{
    if (item.yieldPercent > -ZERO_LIMIT && item.yieldPercent < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (item.yieldPercent > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant itemDailyYieldForegroundRole(const PortfolioItem& item)
{
    if (item.dailyYieldPercent > -ZERO_LIMIT && item.dailyYieldPercent < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (item.dailyYieldPercent > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant itemNothingForegroundRole(const PortfolioItem& /*item*/)
{
    return QVariant();
}

using ItemForegroundRoleHandler = QVariant (*)(const PortfolioItem& item);

static const ItemForegroundRoleHandler ITEM_FOREGROUND_ROLE_HANDLER[PORTFOLIO_COLUMN_COUNT]{
    itemNothingForegroundRole,
    itemNothingForegroundRole,
    itemNothingForegroundRole,
    itemNothingForegroundRole,
    itemNothingForegroundRole,
    itemNothingForegroundRole,
    itemYieldForegroundRole,
    itemYieldPercentForegroundRole,
    itemDailyYieldForegroundRole
};

static QVariant itemAvgPriceTooltipRole(const PortfolioItem& item)
{
    return QObject::tr("Average price by WAVG: %1").arg(item.avgPriceWavg, 0, 'f', item.pricePrecision) + " \u20BD";
}

static QVariant itemYieldPercentTooltipRole(const PortfolioItem& item)
{
    return QObject::tr("From: %1").arg(item.avgPriceFifo, 0, 'f', item.pricePrecision) + " \u20BD";
}

static QVariant itemDailyYieldTooltipRole(const PortfolioItem& item)
{
    return QObject::tr("From: %1").arg(item.priceForDailyYield, 0, 'f', item.pricePrecision) + " \u20BD";
}

static QVariant itemNothingTooltipRole(const PortfolioItem& /*item*/)
{
    return QVariant();
}

using ItemTooltipRoleHandler = QVariant (*)(const PortfolioItem& item);

static const ItemTooltipRoleHandler ITEM_TOOLTIP_ROLE_HANDLER[PORTFOLIO_COLUMN_COUNT]{
    itemNothingTooltipRole,
    itemNothingTooltipRole,
    itemNothingTooltipRole,
    itemAvgPriceTooltipRole,
    itemNothingTooltipRole,
    itemNothingTooltipRole,
    itemNothingTooltipRole,
    itemYieldPercentTooltipRole,
    itemDailyYieldTooltipRole
};

QVariant PortfolioTreeModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const int row    = index.row();
        const int column = index.column();

        PortfolioCategoryItem* category = reinterpret_cast<PortfolioCategoryItem*>(index.internalPointer());

        if (category == nullptr)
        {
            return CATEGORY_DISPLAY_ROLE_HANDLER[column](mPortfolio.positionsList.at(row));
        }

        return ITEM_DISPLAY_ROLE_HANDLER[column](mPortfolio.positionsList.at(category->id).items.at(row));
    }

    if (role == Qt::ForegroundRole)
    {
        PortfolioCategoryItem* category = reinterpret_cast<PortfolioCategoryItem*>(index.internalPointer());

        if (category == nullptr)
        {
            return QVariant();
        }

        const int row    = index.row();
        const int column = index.column();

        return ITEM_FOREGROUND_ROLE_HANDLER[column](mPortfolio.positionsList.at(category->id).items.at(row));
    }

    if (role == Qt::ToolTipRole)
    {
        PortfolioCategoryItem* category = reinterpret_cast<PortfolioCategoryItem*>(index.internalPointer());

        if (category == nullptr)
        {
            return QVariant();
        }

        const int row    = index.row();
        const int column = index.column();

        return ITEM_TOOLTIP_ROLE_HANDLER[column](mPortfolio.positionsList.at(category->id).items.at(row));
    }

    if (role == ROLE_INSTRUMENT_LOGO)
    {
        PortfolioCategoryItem* category = reinterpret_cast<PortfolioCategoryItem*>(index.internalPointer());

        if (category == nullptr)
        {
            return QVariant();
        }

        const int row = index.row();
        Q_ASSERT_X(index.column() == PORTFOLIO_NAME_COLUMN, __FUNCTION__, "Unexpected behavior");

        return reinterpret_cast<qint64>(mPortfolio.positionsList.at(category->id).items.at(row).instrumentLogo);
    }

    if (role == ROLE_INSTRUMENT_NAME)
    {
        PortfolioCategoryItem* category = reinterpret_cast<PortfolioCategoryItem*>(index.internalPointer());

        if (category == nullptr)
        {
            return QVariant();
        }

        const int row = index.row();
        Q_ASSERT_X(index.column() == PORTFOLIO_NAME_COLUMN, __FUNCTION__, "Unexpected behavior");

        return mPortfolio.positionsList.at(category->id).items.at(row).instrumentName;
    }

    return QVariant();
}

void PortfolioTreeModel::sort(int column, Qt::SortOrder order)
{
    if (mSortColumn != column || mSortOrder != order)
    {
        const QList<QPersistentModelIndex> parents;

        emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);

        if (mSortColumn != column)
        {
            mSortColumn = column;
            mSortOrder  = order;

            for (PortfolioCategoryItem& category : mPortfolio.positionsList)
            {
                sortCategory(&category.items);
            }
        }
        else
        {
            mSortOrder = order;

            for (PortfolioCategoryItem& category : mPortfolio.positionsList)
            {
                reverseCategory(&category.items);
            }
        }

        emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
    }
}

void PortfolioTreeModel::portfolioChanged(const Portfolio& portfolio)
{
    beginResetModel();

    mPortfolio = portfolio;

    for (PortfolioCategoryItem& category : mPortfolio.positionsList)
    {
        sortCategory(&category.items);
    }

    endResetModel();
}

void PortfolioTreeModel::lastPriceChanged(const QString& instrumentId, float price)
{
    mLastPricesUpdates[instrumentId] = price;
}

void PortfolioTreeModel::updateLastPrices()
{
    if (!mLastPricesUpdates.isEmpty())
    {
        const QList<QPersistentModelIndex> parents;

        const bool needToSort = mSortColumn == PORTFOLIO_PRICE_COLUMN || mSortColumn == PORTFOLIO_YIELD_COLUMN ||
                                mSortColumn == PORTFOLIO_YIELD_PERCENT_COLUMN || mSortColumn == PORTFOLIO_DAILY_YIELD_COLUMN;

        if (needToSort)
        {
            emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);
        }

        for (PortfolioCategoryItem& category : mPortfolio.positionsList)
        {
            updatePriceInCategory(&category, needToSort);
        }

        mLastPricesUpdates.clear();

        if (needToSort)
        {
            for (PortfolioCategoryItem& category : mPortfolio.positionsList)
            {
                sortCategory(&category.items);
            }

            emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
        }
    }
}

void PortfolioTreeModel::exportToExcel(QXlsx::Document& doc) const
{
    QXlsx::Format categoryStyle;
    categoryStyle.setFontBold(true);
    categoryStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    categoryStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    categoryStyle.setFillPattern(QXlsx::Format::PatternSolid);
    categoryStyle.setPatternBackgroundColor(CATEGORY_BACKGROUND_COLOR);
    categoryStyle.setFontColor(CATEGORY_FONT_COLOR);

    QXlsx::Format categoryRubleStyle;
    categoryRubleStyle.setFontBold(true);
    categoryRubleStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    categoryRubleStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    categoryRubleStyle.setNumberFormat("0.00 \u20BD");
    categoryRubleStyle.setFillPattern(QXlsx::Format::PatternSolid);
    categoryRubleStyle.setPatternBackgroundColor(CATEGORY_BACKGROUND_COLOR);
    categoryRubleStyle.setFontColor(CATEGORY_FONT_COLOR);

    QXlsx::Format categoryPercentStyle;
    categoryPercentStyle.setFontBold(true);
    categoryPercentStyle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    categoryPercentStyle.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    categoryPercentStyle.setNumberFormat("0.00%");
    categoryPercentStyle.setFillPattern(QXlsx::Format::PatternSolid);
    categoryPercentStyle.setPatternBackgroundColor(CATEGORY_BACKGROUND_COLOR);
    categoryPercentStyle.setFontColor(CATEGORY_FONT_COLOR);

    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    int curRow = 2;

    for (const PortfolioCategoryItem& category : mPortfolio.positionsList)
    {
        // clang-format off
        doc.write(curRow, PORTFOLIO_NAME_COLUMN + 1,          category.name, categoryStyle);
        doc.write(curRow, PORTFOLIO_AVAILABLE_COLUMN + 1,     "", categoryStyle);
        doc.write(curRow, PORTFOLIO_PRICE_COLUMN + 1,         "", categoryStyle);
        doc.write(curRow, PORTFOLIO_AVG_PRICE_COLUMN + 1,     "", categoryStyle);
        doc.write(curRow, PORTFOLIO_COST_COLUMN + 1,          category.cost, categoryRubleStyle);
        doc.write(curRow, PORTFOLIO_PART_COLUMN + 1,          category.part / HUNDRED_PERCENT, categoryPercentStyle);
        doc.write(curRow, PORTFOLIO_YIELD_COLUMN + 1,         "", categoryStyle);
        doc.write(curRow, PORTFOLIO_YIELD_PERCENT_COLUMN + 1, "", categoryStyle);
        doc.write(curRow, PORTFOLIO_DAILY_YIELD_COLUMN + 1,   "", categoryStyle);
        // clang-format on

        ++curRow;

        for (const PortfolioItem& item : category.items)
        {
            if (item.showPrices)
            {
                // clang-format off
                doc.write(curRow, PORTFOLIO_NAME_COLUMN + 1,          item.instrumentName,                      cellStyle);
                doc.write(curRow, PORTFOLIO_AVAILABLE_COLUMN + 1,     item.available,                           cellStyle);
                doc.write(curRow, PORTFOLIO_PRICE_COLUMN + 1,         item.price,                               createRubleFormat(CELL_FONT_COLOR, false, item.pricePrecision));
                doc.write(curRow, PORTFOLIO_AVG_PRICE_COLUMN + 1,     item.avgPriceFifo,                        createRubleFormat(CELL_FONT_COLOR, false, item.pricePrecision));
                doc.write(curRow, PORTFOLIO_COST_COLUMN + 1,          item.cost,                                createRubleFormat(CELL_FONT_COLOR, false, 2));
                doc.write(curRow, PORTFOLIO_PART_COLUMN + 1,          item.part / HUNDRED_PERCENT,              createPercentFormat(CELL_FONT_COLOR, false));
                doc.write(curRow, PORTFOLIO_YIELD_COLUMN + 1,         item.yield,                               createRubleFormat(itemYieldForegroundRole(item).value<QBrush>().color(), true, 2));
                doc.write(curRow, PORTFOLIO_YIELD_PERCENT_COLUMN + 1, item.yieldPercent / HUNDRED_PERCENT,      createPercentFormat(itemYieldPercentForegroundRole(item).value<QBrush>().color(), true));
                doc.write(curRow, PORTFOLIO_DAILY_YIELD_COLUMN + 1,   item.dailyYieldPercent / HUNDRED_PERCENT, createPercentFormat(itemDailyYieldForegroundRole(item).value<QBrush>().color(), true));
                // clang-format on
            }
            else
            {
                // clang-format off
                doc.write(curRow, PORTFOLIO_NAME_COLUMN + 1,          item.instrumentName,         cellStyle);
                doc.write(curRow, PORTFOLIO_AVAILABLE_COLUMN + 1,     item.available,              cellStyle);
                doc.write(curRow, PORTFOLIO_PRICE_COLUMN + 1,         "",                          cellStyle);
                doc.write(curRow, PORTFOLIO_AVG_PRICE_COLUMN + 1,     "",                          cellStyle);
                doc.write(curRow, PORTFOLIO_COST_COLUMN + 1,          item.cost,                   createRubleFormat(CELL_FONT_COLOR, false, 2));
                doc.write(curRow, PORTFOLIO_PART_COLUMN + 1,          item.part / HUNDRED_PERCENT, createPercentFormat(CELL_FONT_COLOR, false));
                doc.write(curRow, PORTFOLIO_YIELD_COLUMN + 1,         "",                          cellStyle);
                doc.write(curRow, PORTFOLIO_YIELD_PERCENT_COLUMN + 1, "",                          cellStyle);
                doc.write(curRow, PORTFOLIO_DAILY_YIELD_COLUMN + 1,   "",                          cellStyle);
                // clang-format on
            }

            ++curRow;
        }
    }
}

QXlsx::Format PortfolioTreeModel::createRubleFormat(const QColor& color, bool withPlus, int precision) const
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

QXlsx::Format PortfolioTreeModel::createPercentFormat(const QColor& color, bool withPlus) const
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

static void fillItemsIndeciesForParallel(
    QThread* parentThread, int /*threadId*/, QList<int>& res, int start, int end, void* /*additionalArgs*/
)
{
    int* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = i;
    }
}

struct MergeSortedItemsInfo
{
    explicit MergeSortedItemsInfo(QList<PortfolioItem>* _items, QList<int>* _sortedIndecies) :
        items(_items),
        sortedIndecies(_sortedIndecies)
    {
    }

    QList<PortfolioItem>* items;
    QList<int>*           sortedIndecies;
};

static void mergeSortedItemsForParallel(
    QThread* parentThread, int /*threadId*/, QList<PortfolioItem>& res, int start, int end, void* additionalArgs
)
{
    MergeSortedItemsInfo* mergeSortedItemsInfo = reinterpret_cast<MergeSortedItemsInfo*>(additionalArgs);

    PortfolioItem* itemsArray    = mergeSortedItemsInfo->items->data();
    int*           indeciesArray = mergeSortedItemsInfo->sortedIndecies->data();

    PortfolioItem* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = itemsArray[indeciesArray[i]];
    }
}

void PortfolioTreeModel::sortCategory(QList<PortfolioItem>* items)
{
    QList<int> itemsIndecies;
    itemsIndecies.resizeForOverwrite(items->size());
    processInParallel(itemsIndecies, fillItemsIndeciesForParallel);

    if (mSortOrder == Qt::AscendingOrder)
    {
        if (mSortColumn == PORTFOLIO_NAME_COLUMN)
        {
            const PortfolioTreeNameLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_AVAILABLE_COLUMN)
        {
            const PortfolioTreeAvailableLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_PRICE_COLUMN)
        {
            const PortfolioTreePriceLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_AVG_PRICE_COLUMN)
        {
            const PortfolioTreeAvgPriceLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_COST_COLUMN)
        {
            const PortfolioTreeCostLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_PART_COLUMN)
        {
            const PortfolioTreePartLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_YIELD_COLUMN)
        {
            const PortfolioTreeYieldLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_YIELD_PERCENT_COLUMN)
        {
            const PortfolioTreeYieldPercentLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_DAILY_YIELD_COLUMN)
        {
            const PortfolioTreeDailyYieldLessThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
    }
    else
    {
        if (mSortColumn == PORTFOLIO_NAME_COLUMN)
        {
            const PortfolioTreeNameGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_AVAILABLE_COLUMN)
        {
            const PortfolioTreeAvailableGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_PRICE_COLUMN)
        {
            const PortfolioTreePriceGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_AVG_PRICE_COLUMN)
        {
            const PortfolioTreeAvgPriceGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_COST_COLUMN)
        {
            const PortfolioTreeCostGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_PART_COLUMN)
        {
            const PortfolioTreePartGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_YIELD_COLUMN)
        {
            const PortfolioTreeYieldGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_YIELD_PERCENT_COLUMN)
        {
            const PortfolioTreeYieldPercentGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
        else if (mSortColumn == PORTFOLIO_DAILY_YIELD_COLUMN)
        {
            const PortfolioTreeDailyYieldGreaterThan cmp(items);

            std::stable_sort(std::execution::par, itemsIndecies.begin(), itemsIndecies.end(), cmp);
        }
    }

    QList<PortfolioItem> newItems;
    newItems.resizeForOverwrite(items->size());

    MergeSortedItemsInfo mergeSortedItemsInfo(items, &itemsIndecies);
    processInParallel(newItems, mergeSortedItemsForParallel, &mergeSortedItemsInfo);

    *items = newItems;
}

struct ReverseItemsInfo
{
    explicit ReverseItemsInfo(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;
};

static void reverseItemsForParallel(
    QThread* parentThread, int /*threadId*/, QList<PortfolioItem>& res, int start, int end, void* additionalArgs
)
{
    ReverseItemsInfo* reverseItemsInfo = reinterpret_cast<ReverseItemsInfo*>(additionalArgs);

    PortfolioItem* itemsArray = reverseItemsInfo->items->data();

    PortfolioItem* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = itemsArray[res.size() - i - 1];
    }
}

void PortfolioTreeModel::reverseCategory(QList<PortfolioItem>* items)
{
    QList<PortfolioItem> newItems;
    newItems.resizeForOverwrite(items->size());

    ReverseItemsInfo reverseItemsInfo(items);
    processInParallel(newItems, reverseItemsForParallel, &reverseItemsInfo);

    *items = newItems;
}

void PortfolioTreeModel::updatePriceInCategory(PortfolioCategoryItem* category, bool needToSort)
{
    for (int i = 0; i < category->items.size(); ++i)
    {
        PortfolioItem& item = category->items[i];

        if (mLastPricesUpdates.contains(item.instrumentId))
        {
            item.price = mLastPricesUpdates.value(item.instrumentId);

            const double currentCost = item.available * item.price;

            item.yield             = currentCost - item.cost;
            item.yieldPercent      = (item.yield / item.cost) * HUNDRED_PERCENT;
            item.dailyYield        = currentCost - item.costForDailyYield;
            item.dailyYieldPercent = (item.dailyYield / item.costForDailyYield) * HUNDRED_PERCENT;

            if (!needToSort)
            {
                emit dataChanged(
                    createIndex(i, PORTFOLIO_PRICE_COLUMN, category), createIndex(i, PORTFOLIO_PRICE_COLUMN, category)
                );
                emit dataChanged(
                    createIndex(i, PORTFOLIO_YIELD_COLUMN, category), createIndex(i, PORTFOLIO_DAILY_YIELD_COLUMN, category)
                );
            }
        }
    }
}
