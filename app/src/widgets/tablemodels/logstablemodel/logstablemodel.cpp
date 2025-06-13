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

        return section + 1;
    }

    return QVariant();
}

QVariant LogsTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row    = index.row();
        int column = index.column();

        if (column == LOGS_TIME_COLUMN)
        {
            return QDateTime::fromMSecsSinceEpoch(mEntries.at(row).timestamp).toString(DATETIME_FORMAT);
        }
        else if (column == LOGS_LEVEL_COLUMN)
        {
            return mEntries.at(row).level;
        }
        else if (column == LOGS_NAME_COLUMN)
        {
            return mEntries.at(row).instrumentTicker;
        }
        else if (column == LOGS_MESSAGE_COLUMN)
        {
            return mEntries.at(row).message;
        }
    }
    else if (role == LOGS_ROLE_INSTRUMENT_LOGO)
    {
        int row = index.row();

        Q_ASSERT_X(index.column() == LOGS_NAME_COLUMN, "LogsTableModel::data()", "Unexpected behavior");

        return QVariant::fromValue(reinterpret_cast<void*>(mEntries.at(row).instrumentLogo));
    }
    else if (role == LOGS_ROLE_INSTRUMENT_NAME)
    {
        int row = index.row();

        Q_ASSERT_X(index.column() == LOGS_NAME_COLUMN, "LogsTableModel::data()", "Unexpected behavior");

        return mEntries.at(row).instrumentName;
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
