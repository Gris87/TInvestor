#include "src/widgets/tablemodels/logstablemodel/logstablemodel.h"

#include <QDebug>
#include <algorithm>
#include <execution>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/tablemodels/logstablemodel/comparators.h"
#include "src/widgets/tablemodels/modelroles.h"



const char* const DATETIME_FORMAT       = "yyyy-MM-dd hh:mm:ss";
const QColor      CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



LogsTableModel::LogsTableModel(QObject* parent) :
    ILogsTableModel(parent),
    mHeader(),
    mFilter(),
    mEntriesUnfiltered(std::make_shared<QList<LogEntry>>()),
    mEntries(std::make_shared<QList<LogEntry>>()),
    mSortColumn(LOGS_TIME_COLUMN),
    mSortOrder(Qt::DescendingOrder)
{
    qDebug() << "Create LogsTableModel";

    mHeader << tr("Time") << "L" << tr("Name") << tr("Message");
    Q_ASSERT_X(mHeader.size() == LOGS_COLUMN_COUNT, __FUNCTION__, "Header is incorrect");
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

static QVariant logsTimeDisplayRole(const LogEntry& entry)
{
    return QDateTime::fromMSecsSinceEpoch(entry.timestamp).toString(DATETIME_FORMAT);
}

static QVariant logsLevelDisplayRole(const LogEntry& entry)
{
    return entry.level;
}

static QVariant logsNameDisplayRole(const LogEntry& entry)
{
    return entry.instrumentTicker;
}

static QVariant logsMessageDisplayRole(const LogEntry& entry)
{
    return entry.message;
}

using DisplayRoleHandler = QVariant (*)(const LogEntry& entry);

static const DisplayRoleHandler DISPLAY_ROLE_HANDLER[LOGS_COLUMN_COUNT]{
    logsTimeDisplayRole, logsLevelDisplayRole, logsNameDisplayRole, logsMessageDisplayRole
};

QVariant LogsTableModel::data(const QModelIndex& index, int role) const
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
        Q_ASSERT_X(index.column() == LOGS_NAME_COLUMN, __FUNCTION__, "Unexpected behavior");

        return reinterpret_cast<qint64>(mEntries->at(row).instrumentLogo);
    }

    if (role == ROLE_INSTRUMENT_NAME)
    {
        const int row = index.row();
        Q_ASSERT_X(index.column() == LOGS_NAME_COLUMN, __FUNCTION__, "Unexpected behavior");

        return mEntries->at(row).instrumentName;
    }

    return QVariant();
}

void LogsTableModel::sort(int column, Qt::SortOrder order)
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

    mSortColumn = LOGS_TIME_COLUMN;
    mSortOrder  = Qt::DescendingOrder;

    mEntriesUnfiltered = std::make_shared<QList<LogEntry>>(entries);
    filterAll();

    endResetModel();
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void LogsTableModel::logAdded(const LogEntry& entry)
{
    if (mFilter.isActive())
    {
        if (mSortColumn == LOGS_TIME_COLUMN)
        {
            if (mSortOrder == Qt::AscendingOrder)
            {
                mEntriesUnfiltered->append(entry);

                if (mFilter.isFiltered(entry))
                {
                    insertRow(mEntries.get(), mEntries->size(), entry);
                }
            }
            else
            {
                mEntriesUnfiltered->prepend(entry);

                if (mFilter.isFiltered(entry))
                {
                    insertRow(mEntries.get(), 0, entry);
                }
            }
        }
        else
        {
            mEntriesUnfiltered->insert(indexOfSortedInsert(mEntriesUnfiltered.get(), entry), entry);

            if (mFilter.isFiltered(entry))
            {
                insertRow(mEntries.get(), indexOfSortedInsert(mEntries.get(), entry), entry);
            }
        }
    }
    else
    {
        if (mSortColumn == LOGS_TIME_COLUMN)
        {
            if (mSortOrder == Qt::AscendingOrder)
            {
                insertRow(mEntriesUnfiltered.get(), mEntriesUnfiltered->size(), entry);
            }
            else
            {
                insertRow(mEntriesUnfiltered.get(), 0, entry);
            }
        }
        else
        {
            insertRow(mEntriesUnfiltered.get(), indexOfSortedInsert(mEntriesUnfiltered.get(), entry), entry);
        }
    }
}
// NOLINTEND(readability-function-cognitive-complexity)

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
        const int       row   = i + 2; // Header and start index from 1
        const LogEntry& entry = mEntriesUnfiltered->at(i);

        // clang-format off
        doc.write(row, LOGS_TIME_COLUMN + 1,    QDateTime::fromMSecsSinceEpoch(entry.timestamp), dateFormat);
        doc.write(row, LOGS_LEVEL_COLUMN + 1,   LOG_LEVEL_NAMES[entry.level], cellStyle);
        doc.write(row, LOGS_NAME_COLUMN + 1,    entry.instrumentName, cellStyle);
        doc.write(row, LOGS_MESSAGE_COLUMN + 1, entry.message, cellStyle);
        // clang-format on
    }
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
    explicit MergeSortedEntriesInfo(QList<LogEntry>* _entriesUnfiltered, QList<int>* _sortedIndecies) :
        entriesUnfiltered(_entriesUnfiltered),
        sortedIndecies(_sortedIndecies)
    {
    }

    QList<LogEntry>* entriesUnfiltered;
    QList<int>*      sortedIndecies;
};

static void mergeSortedEntriesForParallel(
    QThread* parentThread, int /*threadId*/, QList<LogEntry>& res, int start, int end, void* additionalArgs
)
{
    MergeSortedEntriesInfo* mergeSortedEntriesInfo = reinterpret_cast<MergeSortedEntriesInfo*>(additionalArgs);

    LogEntry* entriesArray  = mergeSortedEntriesInfo->entriesUnfiltered->data();
    int*      indeciesArray = mergeSortedEntriesInfo->sortedIndecies->data();

    LogEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[indeciesArray[i]];
    }
}

void LogsTableModel::sortEntries()
{
    QList<int> entriesIndecies;
    entriesIndecies.resizeForOverwrite(mEntriesUnfiltered->size());
    processInParallel(entriesIndecies, fillEntriesIndeciesForParallel);

    if (mSortOrder == Qt::AscendingOrder)
    {
        if (mSortColumn == LOGS_TIME_COLUMN)
        {
            const LogsTableTimeLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == LOGS_LEVEL_COLUMN)
        {
            const LogsTableLevelLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == LOGS_NAME_COLUMN)
        {
            const LogsTableNameLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == LOGS_MESSAGE_COLUMN)
        {
            const LogsTableMessageLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
    }
    else
    {
        if (mSortColumn == LOGS_TIME_COLUMN)
        {
            const LogsTableTimeGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == LOGS_LEVEL_COLUMN)
        {
            const LogsTableLevelGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == LOGS_NAME_COLUMN)
        {
            const LogsTableNameGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == LOGS_MESSAGE_COLUMN)
        {
            const LogsTableMessageGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
    }

    const std::shared_ptr<QList<LogEntry>> entries = std::make_shared<QList<LogEntry>>();
    entries->resizeForOverwrite(mEntriesUnfiltered->size());

    MergeSortedEntriesInfo mergeSortedEntriesInfo(mEntriesUnfiltered.get(), &entriesIndecies);
    processInParallel(*entries, mergeSortedEntriesForParallel, &mergeSortedEntriesInfo);

    mEntriesUnfiltered = entries;
}

struct ReverseEntriesInfo
{
    explicit ReverseEntriesInfo(QList<LogEntry>* _entriesUnfiltered) :
        entriesUnfiltered(_entriesUnfiltered)
    {
    }

    QList<LogEntry>* entriesUnfiltered;
};

static void
reverseEntriesForParallel(QThread* parentThread, int /*threadId*/, QList<LogEntry>& res, int start, int end, void* additionalArgs)
{
    ReverseEntriesInfo* reverseEntriesInfo = reinterpret_cast<ReverseEntriesInfo*>(additionalArgs);

    LogEntry* entriesArray = reverseEntriesInfo->entriesUnfiltered->data();

    LogEntry* resArray = res.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[i] = entriesArray[res.size() - i - 1];
    }
}

void LogsTableModel::reverseEntries()
{
    const std::shared_ptr<QList<LogEntry>> entries = std::make_shared<QList<LogEntry>>();
    entries->resizeForOverwrite(mEntriesUnfiltered->size());

    ReverseEntriesInfo reverseEntriesInfo(mEntriesUnfiltered.get());
    processInParallel(*entries, reverseEntriesForParallel, &reverseEntriesInfo);

    mEntriesUnfiltered = entries;
}

using AscSortHandler = bool (*)(const LogEntry& l, const LogEntry& r);

static const AscSortHandler ASC_SORT_HANDLER[LOGS_COLUMN_COUNT]{logsTimeLess, logsLevelLess, logsNameLess, logsMessageLess};

using DescSortHandler = bool (*)(const LogEntry& l, const LogEntry& r);

static const DescSortHandler DESC_SORT_HANDLER[LOGS_COLUMN_COUNT]{
    logsTimeGreater, logsLevelGreater, logsNameGreater, logsMessageGreater
};

int LogsTableModel::indexOfSortedInsert(QList<LogEntry>* entries, const LogEntry& entry)
{
    int res = 0;

    if (mSortOrder == Qt::AscendingOrder)
    {
        res = std::distance(
            entries->begin(), std::lower_bound(entries->begin(), entries->end(), entry, ASC_SORT_HANDLER[mSortColumn])
        );
    }
    else
    {
        res = std::distance(
            entries->begin(), std::lower_bound(entries->begin(), entries->end(), entry, DESC_SORT_HANDLER[mSortColumn])
        );
    }

    return res;
}

void LogsTableModel::insertRow(QList<LogEntry>* entries, int row, const LogEntry& entry)
{
    beginInsertRows(QModelIndex(), row, row);

    entries->insert(row, entry);

    endInsertRows();
}

struct FilterEntriesInfo
{
    explicit FilterEntriesInfo(LogFilter* _filter) :
        filter(_filter)
    {
        const int cpuCount = QThread::idealThreadCount();

        results.resize(cpuCount);
    }

    LogFilter*        filter;
    QList<QList<int>> results;
};

static void filterEntriesForParallel(
    QThread* parentThread, int threadId, QList<LogEntry>& entriesUnfiltered, int start, int end, void* additionalArgs
)
{
    FilterEntriesInfo* filterEntriesInfo = reinterpret_cast<FilterEntriesInfo*>(additionalArgs);

    LogFilter*  filter       = filterEntriesInfo->filter;
    QList<int>* resultsArray = filterEntriesInfo->results.data();

    LogEntry* entriesArray = entriesUnfiltered.data();

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        if (filter->isFiltered(entriesArray[i]))
        {
            resultsArray[threadId].append(i);
        }
    }
}

struct MergeFilteredEntriesInfo
{
    explicit MergeFilteredEntriesInfo(QList<LogEntry>* _entriesUnfiltered, const QList<QList<int>>& _results) :
        entriesUnfiltered(_entriesUnfiltered),
        results(_results)
    {
        indecies.resizeForOverwrite(results.size() + 1);

        int index = 0;

        for (int i = 0; i < results.size(); ++i)
        {
            indecies[i] = index;

            index += results.at(i).size();
        }

        indecies[results.size()] = index;
    }

    QList<LogEntry>*  entriesUnfiltered;
    QList<int>        indecies;
    QList<QList<int>> results;
};

static void mergeFilteredEntriesForParallel(
    QThread* parentThread, int threadId, QList<LogEntry>& res, int /*start*/, int /*end*/, void* additionalArgs
)
{
    MergeFilteredEntriesInfo* mergeFilteredEntriesInfo = reinterpret_cast<MergeFilteredEntriesInfo*>(additionalArgs);

    QList<LogEntry>*  entriesUnfiltered = mergeFilteredEntriesInfo->entriesUnfiltered;
    const int         index             = mergeFilteredEntriesInfo->indecies.at(threadId);
    const QList<int>& results           = mergeFilteredEntriesInfo->results.at(threadId);

    LogEntry* resArray = res.data();

    for (int i = 0; i < results.size() && !parentThread->isInterruptionRequested(); ++i)
    {
        resArray[index + i] = entriesUnfiltered->at(results.at(i));
    }
}

void LogsTableModel::filterAll()
{
    if (mFilter.isActive())
    {
        mEntries = std::make_shared<QList<LogEntry>>();

        FilterEntriesInfo filterEntriesInfo(&mFilter);
        processInParallel(*mEntriesUnfiltered, filterEntriesForParallel, &filterEntriesInfo);

        MergeFilteredEntriesInfo mergeFilteredEntriesInfo(mEntriesUnfiltered.get(), filterEntriesInfo.results);
        mEntries->resizeForOverwrite(mergeFilteredEntriesInfo.indecies.constLast());
        processInParallel(*mEntries, mergeFilteredEntriesForParallel, &mergeFilteredEntriesInfo);
    }
    else
    {
        mEntries = mEntriesUnfiltered;
    }
}
