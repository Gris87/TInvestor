#pragma once



#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel.h"



class LogsTableModel : public ILogsTableModel
{
    Q_OBJECT

public:
    explicit LogsTableModel(QObject* parent = nullptr);
    ~LogsTableModel() override;

    LogsTableModel(const LogsTableModel& another)            = delete;
    LogsTableModel& operator=(const LogsTableModel& another) = delete;

    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void logsRead(const QList<LogEntry>& entries) override;
    void logAdded(const LogEntry& entry) override;

private:
    QStringList     mHeader;
    QList<LogEntry> mEntries;
};
