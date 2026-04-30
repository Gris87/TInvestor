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

QVariant NotificationsTableModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
{
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
