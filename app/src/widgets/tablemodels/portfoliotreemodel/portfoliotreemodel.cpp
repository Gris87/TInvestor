#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodel.h"

#include <QBrush>
#include <QDebug>

#include "src/widgets/tablemodels/modelroles.h"



constexpr float ZERO_LIMIT = 0.0001f;

const QBrush GREEN_COLOR  = QBrush(QColor("#2BD793")); // clazy:exclude=non-pod-global-static
const QBrush RED_COLOR    = QBrush(QColor("#ED6F7E")); // clazy:exclude=non-pod-global-static
const QBrush NORMAL_COLOR = QBrush(QColor("#97AEC4")); // clazy:exclude=non-pod-global-static



PortfolioTreeModel::PortfolioTreeModel(QObject* parent) :
    IPortfolioTreeModel(parent),
    mHeader(),
    mHelpIcon(":/assets/images/question.png"),
    mPortfolio()
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

void PortfolioTreeModel::portfolioChanged(const Portfolio& portfolio)
{
    beginResetModel();

    mPortfolio = portfolio;

    // TODO: Sort

    endResetModel();
}

void PortfolioTreeModel::lastPriceChanged(const QString& /*instrumentId*/, float /*price*/)
{
    // TODO: Implement
}

void PortfolioTreeModel::updateLastPrices()
{
    // TODO: Implement
}
