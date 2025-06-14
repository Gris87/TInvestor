#include "src/widgets/tablemodels/logstablemodel/logstablemodel.h"

#include <QDebug>

#include "src/widgets/tablemodels/logstablemodel/comparators.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";



LogsTableModel::LogsTableModel(QObject* parent) :
    ILogsTableModel(parent),
    mHeader(),
    mFilter(),
    mEntriesUnfiltered(std::make_shared<QList<LogEntry>>()),
    mEntries(std::make_shared<QList<LogEntry>>())
{
    qDebug() << "Create LogsTableModel";

    mHeader << tr("Time") << "L" << tr("Name") << tr("Message");
}

LogsTableModel::~LogsTableModel()
{
    qDebug() << "Destroy LogsTableModel";
}

int LogsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mEntries->size();
}

int LogsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return LOGS_COLUMN_COUNT;
}

QVariant LogsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return mHeader.at(section);
        }

        return section + 1;
    }

    return QVariant();
}

QVariant LogsTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const int row    = index.row();
        const int column = index.column();

        if (column == LOGS_TIME_COLUMN)
        {
            return QDateTime::fromMSecsSinceEpoch(mEntries->at(row).timestamp).toString(DATETIME_FORMAT);
        }

        if (column == LOGS_LEVEL_COLUMN)
        {
            return mEntries->at(row).level;
        }

        if (column == LOGS_NAME_COLUMN)
        {
            return mEntries->at(row).instrumentTicker;
        }

        if (column == LOGS_MESSAGE_COLUMN)
        {
            return mEntries->at(row).message;
        }
    }
    else if (role == LOGS_ROLE_INSTRUMENT_LOGO)
    {
        const int row = index.row();

        Q_ASSERT_X(index.column() == LOGS_NAME_COLUMN, "LogsTableModel::data()", "Unexpected behavior");

        return reinterpret_cast<qint64>(mEntries->at(row).instrumentLogo);
    }
    else if (role == LOGS_ROLE_INSTRUMENT_NAME)
    {
        const int row = index.row();

        Q_ASSERT_X(index.column() == LOGS_NAME_COLUMN, "LogsTableModel::data()", "Unexpected behavior");

        return mEntries->at(row).instrumentName;
    }

    return QVariant();
}

void LogsTableModel::sort(int column, Qt::SortOrder order)
{
    QList<QPersistentModelIndex> parents;

    emit layoutAboutToBeChanged(parents, QAbstractItemModel::VerticalSortHint);

    if (order == Qt::AscendingOrder)
    {
        if (column == LOGS_TIME_COLUMN)
        {
            std::stable_sort(mEntries->begin(), mEntries->end(), LogsTableTimeLessThan());
        }
        else if (column == LOGS_LEVEL_COLUMN)
        {
            std::stable_sort(mEntries->begin(), mEntries->end(), LogsTableLevelLessThan());
        }
        else if (column == LOGS_NAME_COLUMN)
        {
            std::stable_sort(mEntries->begin(), mEntries->end(), LogsTableNameLessThan());
        }
        else if (column == LOGS_MESSAGE_COLUMN)
        {
            std::stable_sort(mEntries->begin(), mEntries->end(), LogsTableMessageLessThan());
        }
    }
    else
    {
        if (column == LOGS_TIME_COLUMN)
        {
            std::stable_sort(mEntries->begin(), mEntries->end(), LogsTableTimeGreaterThan());
        }
        else if (column == LOGS_LEVEL_COLUMN)
        {
            std::stable_sort(mEntries->begin(), mEntries->end(), LogsTableLevelGreaterThan());
        }
        else if (column == LOGS_NAME_COLUMN)
        {
            std::stable_sort(mEntries->begin(), mEntries->end(), LogsTableNameGreaterThan());
        }
        else if (column == LOGS_MESSAGE_COLUMN)
        {
            std::stable_sort(mEntries->begin(), mEntries->end(), LogsTableMessageGreaterThan());
        }
    }

    emit layoutChanged(parents, QAbstractItemModel::VerticalSortHint);
}

void LogsTableModel::setFilter(const LogFilter& filter)
{
    if (mFilter != filter)
    {
        mFilter = filter;

        beginResetModel();
        filterAll();
        endResetModel();
    }
}

void LogsTableModel::logsRead(const QList<LogEntry>& entries)
{
    beginResetModel();

    mEntriesUnfiltered = std::make_shared<QList<LogEntry>>(entries);
    filterAll();

    endResetModel();
}

void LogsTableModel::logAdded(const LogEntry& entry)
{
    if (mFilter.isActive())
    {
        mEntriesUnfiltered->append(entry);

        if (mFilter.isFiltered(entry))
        {
            beginInsertRows(QModelIndex(), mEntries->size(), mEntries->size());

            mEntries->append(entry);

            endInsertRows();
        }
    }
    else
    {
        beginInsertRows(QModelIndex(), mEntriesUnfiltered->size(), mEntriesUnfiltered->size());

        mEntriesUnfiltered->append(entry);

        endInsertRows();
    }
}

void LogsTableModel::filterAll()
{
    if (mFilter.isActive())
    {
        // TODO: Do in parallel
        mEntries = std::make_shared<QList<LogEntry>>();

        for (const LogEntry& entry : std::as_const(*mEntriesUnfiltered))
        {
            if (mFilter.isFiltered(entry))
            {
                mEntries->append(entry);
            }
        }
    }
    else
    {
        mEntries = mEntriesUnfiltered;
    }
}
