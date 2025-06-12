#include "src/widgets/tablemodels/logstablemodel/logstablemodel.h"

#include <QDebug>



LogsTableModel::LogsTableModel(QObject* parent) :
    ILogsTableModel(parent)
{
    qDebug() << "Create LogsTableModel";
}

LogsTableModel::~LogsTableModel()
{
    qDebug() << "Destroy LogsTableModel";
}

int LogsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 100000;
}

int LogsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 4;
}

QVariant LogsTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QString("%1 : %2").arg(index.row()).arg(index.column());
    }

    return QVariant();
}
