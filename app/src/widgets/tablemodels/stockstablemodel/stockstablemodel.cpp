#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"

#include <QBrush>
#include <QDebug>
#include <algorithm>
#include <execution>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/tablemodels/modelroles.h"
#include "src/widgets/tablemodels/stockstablemodel/comparators.h"



constexpr float  ZERO_LIMIT            = 0.0001f;
constexpr qint64 TURNOVER_GREEN_LIMIT  = 1000000000LL;
constexpr qint64 TURNOVER_NORMAL_LIMIT = 1000000LL;
constexpr float  PAYBACK_GREEN_LIMIT   = 60.0f;
constexpr float  PAYBACK_NORMAL_LIMIT  = 30.0f;
constexpr double BILLIONS              = 1000000000.0;
constexpr double MILLIONS              = 1000000.0;
constexpr double KILOS                 = 1000.0;
constexpr float  HUNDRED_PERCENT       = 100.0f;

const QBrush GREEN_COLOR           = QBrush(QColor("#2BD793")); // clazy:exclude=non-pod-global-static
const QBrush RED_COLOR             = QBrush(QColor("#ED6F7E")); // clazy:exclude=non-pod-global-static
const QBrush NORMAL_COLOR          = QBrush(QColor("#97AEC4")); // clazy:exclude=non-pod-global-static
const QColor CELL_BACKGROUND_COLOR = QColor("#2C3C4B");         // clazy:exclude=non-pod-global-static
const QColor CELL_FONT_COLOR       = QColor("#97AEC4");         // clazy:exclude=non-pod-global-static



StocksTableModel::StocksTableModel(QObject* parent) :
    IStocksTableModel(parent),
    lastPricesUpdates(),
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
    return QString::number(entry.price, 'f', entry.pricePrecision) + " \u20BD";
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

static QVariant stocksDayChangeForegroundRole(const StockTableEntry& entry)
{
    if (entry.dayChange > -ZERO_LIMIT && entry.dayChange < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (entry.dayChange > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant stocksDateChangeForegroundRole(const StockTableEntry& entry)
{
    if (entry.dateChange > -ZERO_LIMIT && entry.dateChange < ZERO_LIMIT)
    {
        return NORMAL_COLOR;
    }

    if (entry.dateChange > 0)
    {
        return GREEN_COLOR;
    }

    return RED_COLOR;
}

static QVariant stocksTurnoverForegroundRole(const StockTableEntry& entry)
{
    if (entry.turnover > TURNOVER_GREEN_LIMIT)
    {
        return GREEN_COLOR;
    }

    if (entry.turnover > TURNOVER_NORMAL_LIMIT)
    {
        return NORMAL_COLOR;
    }

    return RED_COLOR;
}

static QVariant stocksPaybackForegroundRole(const StockTableEntry& entry)
{
    if (entry.payback > PAYBACK_GREEN_LIMIT)
    {
        return GREEN_COLOR;
    }

    if (entry.payback > PAYBACK_NORMAL_LIMIT)
    {
        return NORMAL_COLOR;
    }

    return RED_COLOR;
}

static QVariant stocksNothingForegroundRole(const StockTableEntry& /*entry*/)
{
    return QVariant();
}

using ForegroundRoleHandler = QVariant (*)(const StockTableEntry& entry);

static const ForegroundRoleHandler FOREGROUND_ROLE_HANDLER[STOCKS_COLUMN_COUNT]{
    stocksNothingForegroundRole,
    stocksNothingForegroundRole,
    stocksDayChangeForegroundRole,
    stocksDateChangeForegroundRole,
    stocksTurnoverForegroundRole,
    stocksPaybackForegroundRole,
    stocksNothingForegroundRole,
};

static QVariant stocksDayChangeTooltipRole(const StockTableEntry& entry)
{
    if (entry.dayStartPrice <= 0)
    {
        return QVariant();
    }

    return QObject::tr("From: %1").arg(entry.dayStartPrice, 0, 'f', entry.pricePrecision) + " \u20BD";
}

static QVariant stocksDateChangeTooltipRole(const StockTableEntry& entry)
{
    if (entry.specifiedDatePrice <= 0)
    {
        return QVariant();
    }

    return QObject::tr("From: %1").arg(entry.specifiedDatePrice, 0, 'f', entry.pricePrecision) + " \u20BD";
}

static QVariant stocksNothingTooltipRole(const StockTableEntry& /*entry*/)
{
    return QVariant();
}

using TooltipRoleHandler = QVariant (*)(const StockTableEntry& entry);

static const TooltipRoleHandler TOOLTIP_ROLE_HANDLER[STOCKS_COLUMN_COUNT]{
    stocksNothingTooltipRole,
    stocksNothingTooltipRole,
    stocksDayChangeTooltipRole,
    stocksDateChangeTooltipRole,
    stocksNothingTooltipRole,
    stocksNothingTooltipRole,
    stocksNothingTooltipRole,
};

QVariant StocksTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return DISPLAY_ROLE_HANDLER[column](mEntries->at(row));
    }

    if (role == Qt::ForegroundRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return FOREGROUND_ROLE_HANDLER[column](mEntries->at(row));
    }

    if (role == Qt::ToolTipRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return TOOLTIP_ROLE_HANDLER[column](mEntries->at(row));
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

        stock->mutex->lock();

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
        resArray[i].turnover           = stock->operational.turnover;
        resArray[i].payback            = stock->operational.payback;
        resArray[i].dayStartPrice      = stock->operational.dayStartPrice;
        resArray[i].specifiedDatePrice = stock->operational.specifiedDatePrice;
        resArray[i].pricePrecision     = stock->meta.pricePrecision;

        stock->mutex->unlock();
    }
}

void StocksTableModel::updateTable(const QList<Stock*>& stocks)
{
    beginResetModel();

    mEntriesUnfiltered = std::make_shared<QList<StockTableEntry>>();
    mEntriesUnfiltered->resizeForOverwrite(stocks.size());

    FillEntriesInfo fillEntriesInfo(&stocks);
    processInParallel(*mEntriesUnfiltered, fillEntriesForParallel, &fillEntriesInfo);

    mStocks.clear();

    for (Stock* stock : stocks)
    {
        mStocks[stock->meta.instrumentId] = stock;
    }

    sortEntries();
    filterAll();

    endResetModel();
}

struct UpdateAllInfo
{
    explicit UpdateAllInfo(StocksTableModel* _model, const QMap<QString, Stock*>* _stocks, bool _updateAllowed) :
        model(_model),
        stocks(_stocks),
        updateAllowed(_updateAllowed)
    {
    }

    StocksTableModel*            model;
    const QMap<QString, Stock*>* stocks;
    bool                         updateAllowed;
};

static void updateAllForParallel(
    QThread* parentThread, int /*threadId*/, QList<StockTableEntry>& res, int start, int end, void* additionalArgs
)
{
    UpdateAllInfo* updateAllInfo = reinterpret_cast<UpdateAllInfo*>(additionalArgs);

    StocksTableModel*            model         = updateAllInfo->model;
    const QMap<QString, Stock*>* stocks        = updateAllInfo->stocks;
    const bool                   updateAllowed = updateAllInfo->updateAllowed;

    StockTableEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks->value(resArray[i].instrumentId);
        Q_ASSERT_X(stock != nullptr, __FUNCTION__, "Unexpected behavior");

        stock->mutex->lock();

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
        resArray[i].turnover           = stock->operational.turnover;
        resArray[i].payback            = stock->operational.payback;
        resArray[i].dayStartPrice      = stock->operational.dayStartPrice;
        resArray[i].specifiedDatePrice = stock->operational.specifiedDatePrice;
        resArray[i].pricePrecision     = stock->meta.pricePrecision;

        stock->mutex->unlock();

        if (updateAllowed)
        {
            emit model->dataChanged(model->index(i, STOCKS_NAME_COLUMN), model->index(i, STOCKS_PAYBACK_COLUMN));
        }
    }
}

void StocksTableModel::updateAll()
{
    const QList<QPersistentModelIndex> parents;

    const bool needToSort = mSortColumn != STOCKS_ACTIONS_COLUMN;

    if (mFilter.isActive())
    {
        beginResetModel();
    }
    else if (needToSort)
    {
        emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);
    }

    UpdateAllInfo updateAllInfo(this, &mStocks, !mFilter.isActive() && !needToSort);
    processInParallel(*mEntriesUnfiltered, updateAllForParallel, &updateAllInfo);

    if (mFilter.isActive())
    {
        sortEntries();
        filterAll();

        endResetModel();
    }
    else if (needToSort)
    {
        sortEntries();

        emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
    }
}

void StocksTableModel::lastPriceChanged(const QString& instrumentId)
{
    lastPricesUpdates.insert(instrumentId);
}

struct UpdateLastPricesInfo
{
    explicit UpdateLastPricesInfo(StocksTableModel* _model, const QMap<QString, Stock*>* _stocks, bool _updateAllowed) :
        model(_model),
        stocks(_stocks),
        updateAllowed(_updateAllowed)
    {
    }

    StocksTableModel*            model;
    const QMap<QString, Stock*>* stocks;
    bool                         updateAllowed;
};

static void updateLastPricesForParallel(
    QThread* parentThread, int /*threadId*/, QList<StockTableEntry>& res, int start, int end, void* additionalArgs
)
{
    UpdateLastPricesInfo* updateLastPricesInfo = reinterpret_cast<UpdateLastPricesInfo*>(additionalArgs);

    StocksTableModel*            model         = updateLastPricesInfo->model;
    const QMap<QString, Stock*>* stocks        = updateLastPricesInfo->stocks;
    const bool                   updateAllowed = updateLastPricesInfo->updateAllowed;

    StockTableEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        if (!model->lastPricesUpdates.contains(resArray[i].instrumentId))
        {
            continue;
        }

        Stock* stock = stocks->value(resArray[i].instrumentId);
        Q_ASSERT_X(stock != nullptr, __FUNCTION__, "Unexpected behavior");

        stock->mutex->lock();

        resArray[i].price     = stock->lastPrice();
        resArray[i].dayChange = stock->operational.dayStartPrice > 0
                                    ? ((resArray[i].price / stock->operational.dayStartPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                                    : 0;
        resArray[i].dateChange =
            stock->operational.specifiedDatePrice > 0
                ? ((resArray[i].price / stock->operational.specifiedDatePrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                : 0;
        resArray[i].dayStartPrice      = stock->operational.dayStartPrice;
        resArray[i].specifiedDatePrice = stock->operational.specifiedDatePrice;
        resArray[i].pricePrecision     = stock->meta.pricePrecision;

        stock->mutex->unlock();

        if (updateAllowed)
        {
            emit model->dataChanged(model->index(i, STOCKS_PRICE_COLUMN), model->index(i, STOCKS_DATE_CHANGE_COLUMN));
        }
    }
}

void StocksTableModel::updateLastPrices()
{
    if (!lastPricesUpdates.isEmpty())
    {
        const QList<QPersistentModelIndex> parents;

        const bool needToSort = mSortColumn == STOCKS_PRICE_COLUMN || mSortColumn == STOCKS_DAY_CHANGE_COLUMN ||
                                mSortColumn == STOCKS_DATE_CHANGE_COLUMN;

        if (mFilter.isActive())
        {
            beginResetModel();
        }
        else if (needToSort)
        {
            emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);
        }

        UpdateLastPricesInfo updateLastPricesInfo(this, &mStocks, !mFilter.isActive() && !needToSort);
        processInParallel(*mEntriesUnfiltered, updateLastPricesForParallel, &updateLastPricesInfo);

        lastPricesUpdates.clear();

        if (mFilter.isActive())
        {
            sortEntries();
            filterAll();

            endResetModel();
        }
        else if (needToSort)
        {
            sortEntries();

            emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
        }
    }
}

struct UpdatePricesInfo
{
    explicit UpdatePricesInfo(StocksTableModel* _model, const QMap<QString, Stock*>* _stocks, bool _updateAllowed) :
        model(_model),
        stocks(_stocks),
        updateAllowed(_updateAllowed)
    {
    }

    StocksTableModel*            model;
    const QMap<QString, Stock*>* stocks;
    bool                         updateAllowed;
};

static void updatePricesForParallel(
    QThread* parentThread, int /*threadId*/, QList<StockTableEntry>& res, int start, int end, void* additionalArgs
)
{
    UpdatePricesInfo* updatePricesInfo = reinterpret_cast<UpdatePricesInfo*>(additionalArgs);

    StocksTableModel*            model         = updatePricesInfo->model;
    const QMap<QString, Stock*>* stocks        = updatePricesInfo->stocks;
    const bool                   updateAllowed = updatePricesInfo->updateAllowed;

    StockTableEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks->value(resArray[i].instrumentId);
        Q_ASSERT_X(stock != nullptr, __FUNCTION__, "Unexpected behavior");

        stock->mutex->lock();

        resArray[i].price     = stock->lastPrice();
        resArray[i].dayChange = stock->operational.dayStartPrice > 0
                                    ? ((resArray[i].price / stock->operational.dayStartPrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                                    : 0;
        resArray[i].dateChange =
            stock->operational.specifiedDatePrice > 0
                ? ((resArray[i].price / stock->operational.specifiedDatePrice) * HUNDRED_PERCENT) - HUNDRED_PERCENT
                : 0;
        resArray[i].dayStartPrice      = stock->operational.dayStartPrice;
        resArray[i].specifiedDatePrice = stock->operational.specifiedDatePrice;
        resArray[i].pricePrecision     = stock->meta.pricePrecision;

        stock->mutex->unlock();

        if (updateAllowed)
        {
            emit model->dataChanged(model->index(i, STOCKS_PRICE_COLUMN), model->index(i, STOCKS_DATE_CHANGE_COLUMN));
        }
    }
}

void StocksTableModel::updatePrices()
{
    const QList<QPersistentModelIndex> parents;

    const bool needToSort =
        mSortColumn == STOCKS_PRICE_COLUMN || mSortColumn == STOCKS_DAY_CHANGE_COLUMN || mSortColumn == STOCKS_DATE_CHANGE_COLUMN;

    if (mFilter.isActive())
    {
        beginResetModel();
    }
    else if (needToSort)
    {
        emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);
    }

    UpdatePricesInfo updatePricesInfo(this, &mStocks, !mFilter.isActive() && !needToSort);
    processInParallel(*mEntriesUnfiltered, updatePricesForParallel, &updatePricesInfo);

    if (mFilter.isActive())
    {
        sortEntries();
        filterAll();

        endResetModel();
    }
    else if (needToSort)
    {
        sortEntries();

        emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
    }
}

struct UpdatePeriodicDataInfo
{
    explicit UpdatePeriodicDataInfo(StocksTableModel* _model, const QMap<QString, Stock*>* _stocks, bool _updateAllowed) :
        model(_model),
        stocks(_stocks),
        updateAllowed(_updateAllowed)
    {
    }

    StocksTableModel*            model;
    const QMap<QString, Stock*>* stocks;
    bool                         updateAllowed;
};

static void updatePeriodicDataForParallel(
    QThread* parentThread, int /*threadId*/, QList<StockTableEntry>& res, int start, int end, void* additionalArgs
)
{
    UpdatePeriodicDataInfo* updatePeriodicDataInfo = reinterpret_cast<UpdatePeriodicDataInfo*>(additionalArgs);

    StocksTableModel*            model         = updatePeriodicDataInfo->model;
    const QMap<QString, Stock*>* stocks        = updatePeriodicDataInfo->stocks;
    const bool                   updateAllowed = updatePeriodicDataInfo->updateAllowed;

    StockTableEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        Stock* stock = stocks->value(resArray[i].instrumentId);
        Q_ASSERT_X(stock != nullptr, __FUNCTION__, "Unexpected behavior");

        stock->mutex->lock();

        resArray[i].turnover = stock->operational.turnover;
        resArray[i].payback  = stock->operational.payback;

        stock->mutex->unlock();

        if (updateAllowed)
        {
            emit model->dataChanged(model->index(i, STOCKS_TURNOVER_COLUMN), model->index(i, STOCKS_PAYBACK_COLUMN));
        }
    }
}

void StocksTableModel::updatePeriodicData()
{
    const QList<QPersistentModelIndex> parents;

    const bool needToSort = mSortColumn == STOCKS_TURNOVER_COLUMN || mSortColumn == STOCKS_PAYBACK_COLUMN;

    if (mFilter.isActive())
    {
        beginResetModel();
    }
    else if (needToSort)
    {
        emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);
    }

    UpdatePeriodicDataInfo updatePeriodicDataInfo(this, &mStocks, !mFilter.isActive() && !needToSort);
    processInParallel(*mEntriesUnfiltered, updatePeriodicDataForParallel, &updatePeriodicDataInfo);

    if (mFilter.isActive())
    {
        sortEntries();
        filterAll();

        endResetModel();
    }
    else if (needToSort)
    {
        sortEntries();

        emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
    }
}

void StocksTableModel::exportToExcel(QXlsx::Document& doc) const
{
    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    for (int i = 0; i < mEntries->size(); ++i)
    {
        const int              row   = i + 2; // Header and start index from 1
        const StockTableEntry& entry = mEntries->at(i);

        // clang-format off
        doc.write(row, STOCKS_NAME_COLUMN + 1,        entry.instrumentName, cellStyle);
        doc.write(row, STOCKS_NAME_COLUMN + 2,        entry.forQualInvestorFlag, cellStyle);
        doc.write(row, STOCKS_PRICE_COLUMN + 2,       entry.price, createRubleFormat(CELL_FONT_COLOR, entry.pricePrecision));
        doc.write(row, STOCKS_DAY_CHANGE_COLUMN + 2,  entry.dayChange / HUNDRED_PERCENT, createPercentFormat(stocksDayChangeForegroundRole(entry).value<QBrush>().color(), true));
        doc.write(row, STOCKS_DATE_CHANGE_COLUMN + 2, entry.dateChange / HUNDRED_PERCENT, createPercentFormat(stocksDateChangeForegroundRole(entry).value<QBrush>().color(), true));
        doc.write(row, STOCKS_TURNOVER_COLUMN + 2,    entry.turnover, createRubleFormat(stocksTurnoverForegroundRole(entry).value<QBrush>().color(), 0));
        doc.write(row, STOCKS_PAYBACK_COLUMN + 2,     entry.payback / HUNDRED_PERCENT, createPercentFormat(stocksPaybackForegroundRole(entry).value<QBrush>().color(), false));
        // clang-format on
    }
}

QXlsx::Format StocksTableModel::createRubleFormat(const QColor& color, int precision) const
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

QXlsx::Format StocksTableModel::createPercentFormat(const QColor& color, bool withPlus) const
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
