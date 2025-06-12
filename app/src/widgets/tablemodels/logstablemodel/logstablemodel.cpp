#include "src/widgets/tablemodels/logstablemodel/logstablemodel.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";



LogsTableModel::LogsTableModel(QObject* parent) :
    ILogsTableModel(parent),
    mHeader(),
    mEntries()
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
    return mEntries.size();
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
        else
        {
            return section + 1;
        }
    }

    return QVariant();
}

QVariant LogsTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case LOGS_TIME_COLUMN:
            {
                return QDateTime::fromMSecsSinceEpoch(mEntries.at(index.row()).timestamp).toString(DATETIME_FORMAT);
            }
            break;
            case LOGS_LEVEL_COLUMN:
            {
                return mEntries.at(index.row()).level;
            }
            break;
            case LOGS_NAME_COLUMN:
            {
                return mEntries.at(index.row()).instrumentId;
            }
            break;
            case LOGS_MESSAGE_COLUMN:
            {
                return mEntries.at(index.row()).message;
            }
            break;
        }
    }

    return QVariant();
}

void LogsTableModel::logsRead(const QList<LogEntry>& entries)
{
    beginResetModel();

    mEntries = entries;

    endResetModel();
}

void LogsTableModel::logAdded(const LogEntry& entry)
{
    beginInsertRows(QModelIndex(), mEntries.size(), mEntries.size());

    mEntries.append(entry);

    endInsertRows();
}
