#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"

#include <QDebug>
#include <algorithm>
#include <execution>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/tablemodels/modelroles.h"
#include "src/widgets/tablemodels/stockstablemodel/comparators.h"



// const QColor CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
// const QColor CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



StocksTableModel::StocksTableModel(QObject* parent) :
    IStocksTableModel(parent),
    mHeader(),
    mHelpIcon(":/assets/images/question.png"),
    mDateChangeTooltip(),
    mFilter(),
    mEntriesUnfiltered(std::make_shared<QList<Stock*>>()),
    mEntries(std::make_shared<QList<Stock*>>()),
    mSortColumn(STOCKS_STOCK_COLUMN),
    mSortOrder(Qt::AscendingOrder)
{
    qDebug() << "Create StocksTableModel";

    mHeader << tr("Stock") << tr("Price") << tr("Change from day start") << tr("Change from some date") << tr("Turnover")
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

QVariant StocksTableModel::data(const QModelIndex& /*index*/, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return "a";
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

void StocksTableModel::updateTable(const QList<Stock*>& stocks)
{
    beginResetModel();

    mEntriesUnfiltered = std::make_shared<QList<Stock*>>(stocks);
    sortEntries();
    filterAll();

    endResetModel();
}

void StocksTableModel::setDateChangeTooltip(const QString& tooltip)
{
    mDateChangeTooltip = tooltip;
}

void StocksTableModel::sortEntries()
{
    // TODO: Implement
}

void StocksTableModel::reverseEntries()
{
    // TODO: Implement
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
