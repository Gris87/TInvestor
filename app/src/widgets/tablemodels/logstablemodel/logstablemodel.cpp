#include "src/widgets/tablemodels/logstablemodel/logstablemodel.h"

#include <QDebug>

#include "src/widgets/tablemodels/logstablemodel/comparators.h"



const char* const DATETIME_FORMAT       = "yyyy-MM-dd hh:mm:ss";
const QColor      CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



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

void LogsTableModel::exportToExcel(QXlsx::Document& doc) const
{
    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    QXlsx::Format dateFormat;
    dateFormat.setNumberFormat(DATETIME_FORMAT);
    dateFormat.setFillPattern(QXlsx::Format::PatternSolid);
    dateFormat.setBorderStyle(QXlsx::Format::BorderThin);
    dateFormat.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    dateFormat.setFontColor(CELL_FONT_COLOR);

    for (int i = 0; i < mEntriesUnfiltered->size(); ++i)
    {
        int             row   = i + 2; // Header and start index from 1
        const LogEntry& entry = mEntriesUnfiltered->at(i);

        // clang-format off
        doc.write(row, LOGS_TIME_COLUMN + 1,    QDateTime::fromMSecsSinceEpoch(entry.timestamp), dateFormat);
        doc.write(row, LOGS_LEVEL_COLUMN + 1,   LOG_LEVEL_NAMES[entry.level], cellStyle);
        doc.write(row, LOGS_NAME_COLUMN + 1,    entry.instrumentName, cellStyle);
        doc.write(row, LOGS_MESSAGE_COLUMN + 1, entry.message, cellStyle);
        // clang-format on
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
