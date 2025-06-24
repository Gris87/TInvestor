#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"

#include <QDebug>
#include <algorithm>
#include <execution>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/tablemodels/modelroles.h"
#include "src/widgets/tablemodels/stockstablemodel/comparators.h"



// constexpr float ZERO_LIMIT      = 0.0001f;
constexpr qint64 TURNOVER_GREEN_LIMIT  = 1000000000LL;
constexpr qint64 TURNOVER_NORMAL_LIMIT = 1000000LL;
constexpr double BILLIONS              = 1000000000.0;
constexpr double MILLIONS              = 1000000.0;
constexpr double KILOS                 = 1000.0;
constexpr float  HUNDRED_PERCENT       = 100.0f;

// const QBrush      GREEN_COLOR           = QBrush(QColor("#2BD793")); // clazy:exclude=non-pod-global-static
// const QBrush      RED_COLOR             = QBrush(QColor("#ED6F7E")); // clazy:exclude=non-pod-global-static
// const QBrush      NORMAL_COLOR          = QBrush(QColor("#97AEC4")); // clazy:exclude=non-pod-global-static
// const QColor CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
// const QColor CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



StocksTableModel::StocksTableModel(QObject* parent) :
    IStocksTableModel(parent),
    mHeader(),
    mHelpIcon(":/assets/images/question.png"),
    mDateChangeTooltip(),
    mFilter(),
    mEntriesUnfiltered(std::make_shared<QList<StockTableEntry>>()),
    mEntries(std::make_shared<QList<StockTableEntry>>()),
    mSortColumn(STOCKS_NAME_COLUMN),
    mSortOrder(Qt::AscendingOrder)
{
    qDebug() << "Create StocksTableModel";

    mHeader << tr("Name") << tr("Price") << tr("Change from day start") << tr("Change from some date") << tr("Turnover")
            << tr("Payback") << tr("Actions");
    Q_ASSERT_X(mHeader.size() == STOCKS_COLUMN_COUNT, __FUNCTION__, "Header is incorrect");
}

StocksTableModel::~StocksTableModel()
{
    qDebug() << "Destroy StocksTableModel";
}

int StocksTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mEntries->size();
}

int StocksTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return STOCKS_COLUMN_COUNT;
}

QVariant StocksTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        if (orientation == Qt::Horizontal)
        {
            if (section == STOCKS_DATE_CHANGE_COLUMN)
            {
                return mDateChangeTooltip;
            }

            if (section == STOCKS_TURNOVER_COLUMN)
            {
                return tr("Average daily turnover in the last 31 days");
            }

            if (section == STOCKS_PAYBACK_COLUMN)
            {
                return tr("Opportunity to get money back (with commission) based on the last 24 hours");
            }
        }

        return QVariant();
    }

    if (role == Qt::DecorationRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == STOCKS_DATE_CHANGE_COLUMN || section == STOCKS_TURNOVER_COLUMN || section == STOCKS_PAYBACK_COLUMN)
            {
                return mHelpIcon;
            }
        }

        return QVariant();
    }

    return QVariant();
}

static QVariant stocksNameDisplayRole(const StockTableEntry& entry)
{
    return entry.instrumentTicker;
}

static QVariant stocksPriceDisplayRole(const StockTableEntry& entry)
{
    return QString::number(entry.price, 'f', 2) + " \u20BD"; // TODO: Use stock pricePrecision
}

static QVariant stocksDayChangeDisplayRole(const StockTableEntry& entry)
{
    const QString prefix = entry.dayChange > 0 ? "+" : "";

    return prefix + QString::number(entry.dayChange, 'f', 2) + "%";
}

static QVariant stocksDateChangeDisplayRole(const StockTableEntry& entry)
{
    const QString prefix = entry.dateChange > 0 ? "+" : "";

    return prefix + QString::number(entry.dateChange, 'f', 2) + "%";
}

static QVariant stocksTurnoverDisplayRole(const StockTableEntry& entry)
{
    QString text;

    if (entry.turnover >= TURNOVER_GREEN_LIMIT)
    {
        text = QString::number(static_cast<double>(entry.turnover) / BILLIONS, 'f', 2) + "B \u20BD";
    }
    else if (entry.turnover >= TURNOVER_NORMAL_LIMIT)
    {
        text = QString::number(static_cast<double>(entry.turnover) / MILLIONS, 'f', 2) + "M \u20BD";
    }
    else
    {
        text = QString::number(static_cast<double>(entry.turnover) / KILOS, 'f', 2) + "K \u20BD";
    }

    return text;
}

static QVariant stocksPaybackDisplayRole(const StockTableEntry& entry)
{
    return QString::number(entry.payback, 'f', 2) + "%";
}

static QVariant stocksActionsDisplayRole(const StockTableEntry& /*entry*/)
{
    return QVariant();
}

using DisplayRoleHandler = QVariant (*)(const StockTableEntry& entry);

static const DisplayRoleHandler DISPLAY_ROLE_HANDLER[STOCKS_COLUMN_COUNT]{
    stocksNameDisplayRole,
    stocksPriceDisplayRole,
    stocksDayChangeDisplayRole,
    stocksDateChangeDisplayRole,
    stocksTurnoverDisplayRole,
    stocksPaybackDisplayRole,
    stocksActionsDisplayRole,
};

QVariant StocksTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return DISPLAY_ROLE_HANDLER[column](mEntries->at(row));
    }

    if (role == ROLE_INSTRUMENT_LOGO)
    {
        const int row = index.row();
        Q_ASSERT_X(index.column() == STOCKS_NAME_COLUMN, __FUNCTION__, "Unexpected behavior");

        return reinterpret_cast<qint64>(mEntries->at(row).instrumentLogo);
    }

    if (role == ROLE_INSTRUMENT_NAME)
    {
        const int row = index.row();
        Q_ASSERT_X(index.column() == STOCKS_NAME_COLUMN, __FUNCTION__, "Unexpected behavior");

        return mEntries->at(row).instrumentName;
    }

    return QVariant();
}

void StocksTableModel::sort(int column, Qt::SortOrder order)
{
    if (mSortColumn != column || mSortOrder != order)
    {
        const QList<QPersistentModelIndex> parents;

        emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);

        if (mSortColumn != column)
        {
            mSortColumn = column;
            mSortOrder  = order;

            sortEntries();
        }
        else
        {
            mSortOrder = order;

            reverseEntries();
        }

        filterAll();

        emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
    }
}

void StocksTableModel::setFilter(const StockFilter& filter)
{
    if (mFilter != filter)
    {
        mFilter = filter;

        beginResetModel();
        filterAll();
        endResetModel();
    }
}

struct FillEntriesInfo
{
    explicit FillEntriesInfo(const QList<Stock*>* _stocks) :
        stocks(_stocks)
    {
    }

    const QList<Stock*>* stocks;
};

static void fillEntriesForParallel(
    QThread* parentThread, int /*threadId*/, QList<StockTableEntry>& res, int start, int end, void* additionalArgs
)
{
    FillEntriesInfo* fillEntriesInfo = reinterpret_cast<FillEntriesInfo*>(additionalArgs);

    Stock* const* stocksArray = fillEntriesInfo->stocks->data();

    StockTableEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocksArray[i];

        resArray[i].instrumentId        = stock->meta.instrumentId;
        resArray[i].instrumentLogo      = stock->meta.instrumentLogo;
        resArray[i].instrumentTicker    = stock->meta.instrumentTicker;
        resArray[i].instrumentName      = stock->meta.instrumentName;
        resArray[i].forQualInvestorFlag = stock->meta.forQualInvestorFlag;
        resArray[i].price               = stock->lastPrice();
        resArray[i].dayChange           = stock->operational.dayStartPrice > 0
                                              ? ((resArray[i].price / stock->operational.dayStartPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                                              : 0;
        resArray[i].dateChange =
            stock->operational.specifiedDatePrice > 0
                ? ((resArray[i].price / stock->operational.specifiedDatePrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                : 0;
        resArray[i].turnover       = stock->operational.turnover;
        resArray[i].payback        = stock->operational.payback;
        resArray[i].pricePrecision = stock->meta.pricePrecision;
    }
}

void StocksTableModel::updateTable(const QList<Stock*>& stocks)
{
    beginResetModel();

    mEntriesUnfiltered = std::make_shared<QList<StockTableEntry>>();
    mEntriesUnfiltered->resizeForOverwrite(stocks.size());

    FillEntriesInfo fillEntriesInfo(&stocks);
    processInParallel(*mEntriesUnfiltered, fillEntriesForParallel, &fillEntriesInfo);

    sortEntries();
    filterAll();

    endResetModel();
}

void StocksTableModel::setDateChangeTooltip(const QString& tooltip)
{
    mDateChangeTooltip = tooltip;
}

static void fillEntriesIndeciesForParallel(
    QThread* parentThread, int /*threadId*/, QList<int>& res, int start, int end, void* /*additionalArgs*/
)
{
    int* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = i;
    }
}

struct MergeSortedEntriesInfo
{
    explicit MergeSortedEntriesInfo(QList<StockTableEntry>* _entriesUnfiltered, QList<int>* _sortedIndecies) :
        entriesUnfiltered(_entriesUnfiltered),
        sortedIndecies(_sortedIndecies)
    {
    }

    QList<StockTableEntry>* entriesUnfiltered;
    QList<int>*             sortedIndecies;
};

static void mergeSortedEntriesForParallel(
    QThread* parentThread, int /*threadId*/, QList<StockTableEntry>& res, int start, int end, void* additionalArgs
)
{
    MergeSortedEntriesInfo* mergeSortedEntriesInfo = reinterpret_cast<MergeSortedEntriesInfo*>(additionalArgs);

    StockTableEntry* entriesArray  = mergeSortedEntriesInfo->entriesUnfiltered->data();
    int*             indeciesArray = mergeSortedEntriesInfo->sortedIndecies->data();

    StockTableEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[indeciesArray[i]];
    }
}

void StocksTableModel::sortEntries()
{
    QList<int> entriesIndecies;
    entriesIndecies.resizeForOverwrite(mEntriesUnfiltered->size());
    processInParallel(entriesIndecies, fillEntriesIndeciesForParallel);

    if (mSortOrder == Qt::AscendingOrder)
    {
        if (mSortColumn == STOCKS_NAME_COLUMN)
        {
            const StocksTableNameLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_PRICE_COLUMN)
        {
            const StocksTablePriceLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_DAY_CHANGE_COLUMN)
        {
            const StocksTableDayChangeLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_DATE_CHANGE_COLUMN)
        {
            const StocksTableDateChangeLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_TURNOVER_COLUMN)
        {
            const StocksTableTurnoverLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_PAYBACK_COLUMN)
        {
            const StocksTablePaybackLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
    }
    else
    {
        if (mSortColumn == STOCKS_NAME_COLUMN)
        {
            const StocksTableNameGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_PRICE_COLUMN)
        {
            const StocksTablePriceGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_DAY_CHANGE_COLUMN)
        {
            const StocksTableDayChangeGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_DATE_CHANGE_COLUMN)
        {
            const StocksTableDateChangeGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_TURNOVER_COLUMN)
        {
            const StocksTableTurnoverGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == STOCKS_PAYBACK_COLUMN)
        {
            const StocksTablePaybackGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
    }

    const std::shared_ptr<QList<StockTableEntry>> entries = std::make_shared<QList<StockTableEntry>>();
    entries->resizeForOverwrite(mEntriesUnfiltered->size());

    MergeSortedEntriesInfo mergeSortedEntriesInfo(mEntriesUnfiltered.get(), &entriesIndecies);
    processInParallel(*entries, mergeSortedEntriesForParallel, &mergeSortedEntriesInfo);

    mEntriesUnfiltered = entries;
}

struct ReverseEntriesInfo
{
    explicit ReverseEntriesInfo(QList<StockTableEntry>* _entriesUnfiltered) :
        entriesUnfiltered(_entriesUnfiltered)
    {
    }

    QList<StockTableEntry>* entriesUnfiltered;
};

static void reverseEntriesForParallel(
    QThread* parentThread, int /*threadId*/, QList<StockTableEntry>& res, int start, int end, void* additionalArgs
)
{
    ReverseEntriesInfo* reverseEntriesInfo = reinterpret_cast<ReverseEntriesInfo*>(additionalArgs);

    StockTableEntry* entriesArray = reverseEntriesInfo->entriesUnfiltered->data();

    StockTableEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[res.size() - i - 1];
    }
}

void StocksTableModel::reverseEntries()
{
    const std::shared_ptr<QList<StockTableEntry>> entries = std::make_shared<QList<StockTableEntry>>();
    entries->resizeForOverwrite(mEntriesUnfiltered->size());

    ReverseEntriesInfo reverseEntriesInfo(mEntriesUnfiltered.get());
    processInParallel(*entries, reverseEntriesForParallel, &reverseEntriesInfo);

    mEntriesUnfiltered = entries;
}

void StocksTableModel::filterAll()
{
    if (mFilter.isActive())
    {
        // TODO: Implement
    }
    else
    {
        mEntries = mEntriesUnfiltered;
    }
}
