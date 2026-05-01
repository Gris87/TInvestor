#include "src/widgets/tablemodels/notificationstablemodel/notificationstablemodel.h"

#include <QDebug>
#include <algorithm>
#include <execution>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/tablemodels/notificationstablemodel/comparators.h"



NotificationsTableModel::NotificationsTableModel(QObject* parent) :
    INotificationsTableModel(parent)
{
    qDebug() << "Create NotificationsTableModel";

    mHeader << tr("Time") << tr("Type") << tr("Text");
    Q_ASSERT_X(mHeader.size() == NOTIFICATIONS_COLUMN_COUNT, __FUNCTION__, "Header is incorrect");
}

NotificationsTableModel::~NotificationsTableModel()
{
    qDebug() << "Destroy NotificationsTableModel";
}

int NotificationsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 0;
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

QVariant NotificationsTableModel::data(const QModelIndex& /*index*/, int /*role*/) const
{
    return QVariant();
}

void NotificationsTableModel::sort(int /*column*/, Qt::SortOrder /*order*/)
{
}

void NotificationsTableModel::setFilter(const Filter& /*filter*/)
{
}

void NotificationsTableModel::notificationsRead(const QList<NotificationInfo>& /*notifications*/)
{
}

void NotificationsTableModel::notificationsAdded(const QList<NotificationInfo>& /*notifications*/)
{
}
