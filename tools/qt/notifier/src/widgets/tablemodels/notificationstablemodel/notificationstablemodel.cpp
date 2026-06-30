#include "src/widgets/tablemodels/notificationstablemodel/notificationstablemodel.h"

#include <QBrush>
#include <QCoreApplication>
#include <QDebug>
#include <QFont>
#include <algorithm>
#include <execution>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/tablemodels/modelroles.h"
#include "src/widgets/tablemodels/notificationstablemodel/comparators.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";
const QColor      HIGHLIGHT_COLOR = QColor("#918A00"); // clazy:exclude=non-pod-global-static
const QColor      NORMAL_COLOR    = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QBrush      LINK_COLOR      = QBrush(QColor("#008CFF")); // clazy:exclude=non-pod-global-static

constexpr qint64 MS_IN_SECOND    = 1000LL;
constexpr qint64 ONE_MINUTE      = 60LL * MS_IN_SECOND;
constexpr qint64 HIGHLIGHT_DELAY = 15LL * ONE_MINUTE;



NotificationsTableModel::NotificationsTableModel(QObject* parent) :
    INotificationsTableModel(parent),
    mHeader(),
    mFilter(),
    mEntriesUnfiltered(std::make_shared<QList<NotificationInfo>>()),
    mEntries(std::make_shared<QList<NotificationInfo>>()),
    mSortColumn(NOTIFICATIONS_TIME_COLUMN),
    mSortOrder(Qt::DescendingOrder)
{
    qDebug() << "Create NotificationsTableModel";

    mHeader << tr("Time") << tr("Type") << tr("Text");
    Q_ASSERT_X(mHeader.size() == NOTIFICATIONS_COLUMN_COUNT, __FUNCTION__, "Header is incorrect");

    mMessageTypes << "" << tr("System") << tr("Portfolio") << tr("Huge sell") << tr("Dividends") << tr("Pulse (Neutral)")
                  << tr("Pulse (Buy)") << tr("Pulse (Sell)");
    Q_ASSERT_X(mMessageTypes.size() == MESSAGE_TYPE_COUNT, __FUNCTION__, "Header is incorrect");
}

NotificationsTableModel::~NotificationsTableModel()
{
    qDebug() << "Destroy NotificationsTableModel";
}

int NotificationsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return mEntries->size();
}

int NotificationsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return NOTIFICATIONS_COLUMN_COUNT;
}

QVariant NotificationsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

static QVariant notificationsTimeDisplayRole(const NotificationInfo& notification, const QStringList& /*messageTypes*/)
{
    return QDateTime::fromMSecsSinceEpoch(notification.timestamp).toString(DATETIME_FORMAT);
}

static QVariant notificationsTypeDisplayRole(const NotificationInfo& notification, const QStringList& messageTypes)
{
    return messageTypes.at(notification.messageType);
}

static QVariant notificationsTextDisplayRole(const NotificationInfo& notification, const QStringList& /*messageTypes*/)
{
    return notification.text;
}

using DisplayRoleHandler = QVariant (*)(const NotificationInfo& notification, const QStringList& messageTypes);

static const DisplayRoleHandler DISPLAY_ROLE_HANDLER[NOTIFICATIONS_COLUMN_COUNT]{
    notificationsTimeDisplayRole, notificationsTypeDisplayRole, notificationsTextDisplayRole
};

static QVariant notificationsTextForegroundRole(const NotificationInfo& notification)
{
    return notification.data != "" ? LINK_COLOR : QVariant();
}

static QVariant notificationsNothingForegroundRole(const NotificationInfo& /*notification*/)
{
    return QVariant();
}

using ForegroundRoleHandler = QVariant (*)(const NotificationInfo& notification);

static const ForegroundRoleHandler FOREGROUND_ROLE_HANDLER[NOTIFICATIONS_COLUMN_COUNT]{
    notificationsNothingForegroundRole,
    notificationsNothingForegroundRole,
    notificationsTextForegroundRole
};

static QVariant notificationsTextFontRole(const NotificationInfo& notification)
{
    if (notification.data == "")
    {
        return QVariant();
    }

    QFont font;
    font.setUnderline(true);

    return font;
}

static QVariant notificationsNothingFontRole(const NotificationInfo& /*notification*/)
{
    return QVariant();
}

using FontRoleHandler = QVariant (*)(const NotificationInfo& notification);

static const FontRoleHandler FONT_ROLE_HANDLER[NOTIFICATIONS_COLUMN_COUNT]{
    notificationsNothingFontRole,
    notificationsNothingFontRole,
    notificationsTextFontRole
};

QVariant NotificationsTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return DISPLAY_ROLE_HANDLER[column](mEntries->at(row), mMessageTypes);
    }

    if (role == Qt::BackgroundRole)
    {
        const int               row          = index.row();
        const NotificationInfo& notification = mEntries->at(row);

        const qint64 delta = qMax(QDateTime::currentMSecsSinceEpoch() - notification.requestTimestamp, 0);

        if (delta <= HIGHLIGHT_DELAY)
        {
            // clang-format off
            const int r = HIGHLIGHT_COLOR.red()   + (((NORMAL_COLOR.red()   - HIGHLIGHT_COLOR.red())   * delta) / HIGHLIGHT_DELAY);
            const int g = HIGHLIGHT_COLOR.green() + (((NORMAL_COLOR.green() - HIGHLIGHT_COLOR.green()) * delta) / HIGHLIGHT_DELAY);
            const int b = HIGHLIGHT_COLOR.blue()  + (((NORMAL_COLOR.blue()  - HIGHLIGHT_COLOR.blue())  * delta) / HIGHLIGHT_DELAY);
            // clang-format on

            return QBrush(QColor(r, g, b));
        }

        return QVariant();
    }

    if (role == Qt::ForegroundRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return FOREGROUND_ROLE_HANDLER[column](mEntries->at(row));
    }

    if (role == Qt::FontRole)
    {
        const int row    = index.row();
        const int column = index.column();

        return FONT_ROLE_HANDLER[column](mEntries->at(row));
    }

    if (role == ROLE_URL)
    {
        const int row = index.row();
        Q_ASSERT_X(index.column() == NOTIFICATIONS_TEXT_COLUMN, __FUNCTION__, "Unexpected behavior");

        return mEntries->at(row).data != "" ? QString("%1/data/attachments/%2.txt")
                                                  .arg(qApp->applicationDirPath(), QString::number(mEntries->at(row).timestamp))
                                            : "";
    }

    return QVariant();
}

void NotificationsTableModel::sort(int column, Qt::SortOrder order)
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

void NotificationsTableModel::setFilter(const Filter& filter)
{
    if (mFilter != filter)
    {
        mFilter = filter;

        beginResetModel();
        filterAll();
        endResetModel();
    }
}

void NotificationsTableModel::notificationsRead(const QList<NotificationInfo>& notifications)
{
    beginResetModel();

    mSortColumn = NOTIFICATIONS_TIME_COLUMN;
    mSortOrder  = Qt::DescendingOrder;

    mEntriesUnfiltered = std::make_shared<QList<NotificationInfo>>(notifications);
    filterAll();

    endResetModel();
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void NotificationsTableModel::notificationsAdded(const QList<NotificationInfo>& notifications)
{
    for (int i = notifications.size() - 1; i >= 0; --i)
    {
        const NotificationInfo& notification = notifications.at(i);

        if (mFilter.isActive())
        {
            if (mSortColumn == NOTIFICATIONS_TIME_COLUMN)
            {
                if (mSortOrder == Qt::AscendingOrder)
                {
                    mEntriesUnfiltered->append(notification);

                    if (mFilter.isFiltered(notification))
                    {
                        insertRow(mEntries.get(), mEntries->size(), notification);
                    }
                }
                else
                {
                    mEntriesUnfiltered->prepend(notification);

                    if (mFilter.isFiltered(notification))
                    {
                        insertRow(mEntries.get(), 0, notification);
                    }
                }
            }
            else
            {
                mEntriesUnfiltered->insert(indexOfSortedInsert(mEntriesUnfiltered.get(), notification), notification);

                if (mFilter.isFiltered(notification))
                {
                    insertRow(mEntries.get(), indexOfSortedInsert(mEntries.get(), notification), notification);
                }
            }
        }
        else
        {
            if (mSortColumn == NOTIFICATIONS_TIME_COLUMN)
            {
                if (mSortOrder == Qt::AscendingOrder)
                {
                    insertRow(mEntriesUnfiltered.get(), mEntriesUnfiltered->size(), notification);
                }
                else
                {
                    insertRow(mEntriesUnfiltered.get(), 0, notification);
                }
            }
            else
            {
                insertRow(mEntriesUnfiltered.get(), indexOfSortedInsert(mEntriesUnfiltered.get(), notification), notification);
            }
        }
    }
}
// NOLINTEND(readability-function-cognitive-complexity)

void NotificationsTableModel::refreshBackground()
{
    beginResetModel();
    endResetModel();
}

static void fillEntriesIndeciesForParallel(
    QThread* parentThread, int /*threadId*/, int* res, int /*size*/, int start, int end, void* /*additionalArgs*/
)
{
    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        res[i] = i;
    }
}

struct MergeSortedEntriesInfo
{
    explicit MergeSortedEntriesInfo(const QList<NotificationInfo>& _entriesUnfiltered, const QList<int>& _sortedIndecies)
    {
        entriesArray  = _entriesUnfiltered.constData();
        indeciesArray = _sortedIndecies.constData();
    }

    const NotificationInfo* entriesArray;
    const int*              indeciesArray;
};

static void mergeSortedEntriesForParallel(
    QThread* parentThread, int /*threadId*/, NotificationInfo* res, int /*size*/, int start, int end, void* additionalArgs
)
{
    const MergeSortedEntriesInfo* mergeSortedEntriesInfo = reinterpret_cast<MergeSortedEntriesInfo*>(additionalArgs);

    const NotificationInfo* entriesArray  = mergeSortedEntriesInfo->entriesArray;
    const int*              indeciesArray = mergeSortedEntriesInfo->indeciesArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        res[i] = entriesArray[indeciesArray[i]];
    }
}

void NotificationsTableModel::sortEntries()
{
    QList<int> entriesIndecies;
    entriesIndecies.resizeForOverwrite(mEntriesUnfiltered->size());
    processInParallel(QThread::currentThread(), entriesIndecies, fillEntriesIndeciesForParallel);

    if (mSortOrder == Qt::AscendingOrder)
    {
        if (mSortColumn == NOTIFICATIONS_TIME_COLUMN)
        {
            const NotificationsTableTimeLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == NOTIFICATIONS_TYPE_COLUMN)
        {
            const NotificationsTableTypeLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == NOTIFICATIONS_TEXT_COLUMN)
        {
            const NotificationsTableTextLessThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
    }
    else
    {
        if (mSortColumn == NOTIFICATIONS_TIME_COLUMN)
        {
            const NotificationsTableTimeGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == NOTIFICATIONS_TYPE_COLUMN)
        {
            const NotificationsTableTypeGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
        else if (mSortColumn == NOTIFICATIONS_TEXT_COLUMN)
        {
            const NotificationsTableTextGreaterThan cmp(mEntriesUnfiltered.get());

            std::stable_sort(std::execution::par, entriesIndecies.begin(), entriesIndecies.end(), cmp);
        }
    }

    const std::shared_ptr<QList<NotificationInfo>> entries = std::make_shared<QList<NotificationInfo>>();
    entries->resizeForOverwrite(mEntriesUnfiltered->size());

    MergeSortedEntriesInfo mergeSortedEntriesInfo(*mEntriesUnfiltered, entriesIndecies);
    processInParallel(QThread::currentThread(), *entries, mergeSortedEntriesForParallel, &mergeSortedEntriesInfo);

    mEntriesUnfiltered = entries;
}

struct ReverseEntriesInfo
{
    explicit ReverseEntriesInfo(const QList<NotificationInfo>& _entriesUnfiltered)
    {
        entriesArray = _entriesUnfiltered.constData();
    }

    const NotificationInfo* entriesArray;
};

static void reverseEntriesForParallel(
    QThread* parentThread, int /*threadId*/, NotificationInfo* res, int size, int start, int end, void* additionalArgs
)
{
    const ReverseEntriesInfo* reverseEntriesInfo = reinterpret_cast<ReverseEntriesInfo*>(additionalArgs);

    const NotificationInfo* entriesArray = reverseEntriesInfo->entriesArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        res[i] = entriesArray[size - i - 1];
    }
}

void NotificationsTableModel::reverseEntries()
{
    const std::shared_ptr<QList<NotificationInfo>> entries = std::make_shared<QList<NotificationInfo>>();
    entries->resizeForOverwrite(mEntriesUnfiltered->size());

    ReverseEntriesInfo reverseEntriesInfo(*mEntriesUnfiltered);
    processInParallel(QThread::currentThread(), *entries, reverseEntriesForParallel, &reverseEntriesInfo);

    mEntriesUnfiltered = entries;
}

using AscSortHandler = bool (*)(const NotificationInfo& l, const NotificationInfo& r);

static const AscSortHandler ASC_SORT_HANDLER[NOTIFICATIONS_COLUMN_COUNT]{
    nullptr, // Never used
    notificationsTypeLess,
    notificationsTextLess
};

using DescSortHandler = bool (*)(const NotificationInfo& l, const NotificationInfo& r);

static const DescSortHandler DESC_SORT_HANDLER[NOTIFICATIONS_COLUMN_COUNT]{
    nullptr, // Never used
    notificationsTypeGreater,
    notificationsTextGreater
};

int NotificationsTableModel::indexOfSortedInsert(QList<NotificationInfo>* entries, const NotificationInfo& entry)
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

void NotificationsTableModel::insertRow(QList<NotificationInfo>* entries, int row, const NotificationInfo& entry)
{
    beginInsertRows(QModelIndex(), row, row);

    entries->insert(row, entry);

    endInsertRows();
}

struct FilterEntriesInfo
{
    explicit FilterEntriesInfo(Filter* _filter) :
        filter(_filter)
    {
        results.resize(getCpuCount());
        resultsArray = results.data();
    }

    Filter*           filter;
    QList<QList<int>> results;
    QList<int>*       resultsArray;
};

static void filterEntriesForParallel(
    QThread*          parentThread,
    int               threadId,
    NotificationInfo* entriesUnfiltered,
    int /*size*/,
    int   start,
    int   end,
    void* additionalArgs
)
{
    const FilterEntriesInfo* filterEntriesInfo = reinterpret_cast<FilterEntriesInfo*>(additionalArgs);

    const Filter* filter       = filterEntriesInfo->filter;
    QList<int>*   resultsArray = filterEntriesInfo->resultsArray;

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        if (filter->isFiltered(entriesUnfiltered[i]))
        {
            resultsArray[threadId].append(i);
        }
    }
}

struct MergeFilteredEntriesInfo
{
    explicit MergeFilteredEntriesInfo(const QList<NotificationInfo>& _entriesUnfiltered, const QList<QList<int>>& _results)
    {
        entriesUnfilteredArray = _entriesUnfiltered.constData();
        indecies.resizeForOverwrite(_results.size() + 1);
        indeciesArray = indecies.data();
        resultsArray  = _results.constData();

        int index = 0;

        for (int i = 0; i < _results.size(); ++i)
        {
            indeciesArray[i] = index;

            index += resultsArray[i].size();
        }

        indeciesArray[_results.size()] = index;
    }

    const NotificationInfo* entriesUnfilteredArray;
    QList<int>              indecies;
    int*                    indeciesArray;
    const QList<int>*       resultsArray;
};

static void mergeFilteredEntriesForParallel(
    QThread* parentThread, int threadId, NotificationInfo* res, int /*size*/, int /*start*/, int /*end*/, void* additionalArgs
)
{
    const MergeFilteredEntriesInfo* mergeFilteredEntriesInfo = reinterpret_cast<MergeFilteredEntriesInfo*>(additionalArgs);

    const NotificationInfo* entriesUnfilteredArray = mergeFilteredEntriesInfo->entriesUnfilteredArray;
    const int               index                  = mergeFilteredEntriesInfo->indeciesArray[threadId];
    const QList<int>&       results                = mergeFilteredEntriesInfo->resultsArray[threadId];

    const int* resultsArray = results.constData();
    const int  resultsSize  = results.size();

    for (int i = 0; i < resultsSize && !parentThread->isInterruptionRequested(); ++i)
    {
        res[index + i] = entriesUnfilteredArray[resultsArray[i]];
    }
}

void NotificationsTableModel::filterAll()
{
    if (mFilter.isActive())
    {
        mEntries = std::make_shared<QList<NotificationInfo>>();

        FilterEntriesInfo filterEntriesInfo(&mFilter);
        processInParallel(QThread::currentThread(), *mEntriesUnfiltered, filterEntriesForParallel, &filterEntriesInfo);

        MergeFilteredEntriesInfo mergeFilteredEntriesInfo(*mEntriesUnfiltered, filterEntriesInfo.results);
        mEntries->resizeForOverwrite(mergeFilteredEntriesInfo.indecies.constLast());
        processInParallel(QThread::currentThread(), *mEntries, mergeFilteredEntriesForParallel, &mergeFilteredEntriesInfo);
    }
    else
    {
        mEntries = mEntriesUnfiltered;
    }
}
