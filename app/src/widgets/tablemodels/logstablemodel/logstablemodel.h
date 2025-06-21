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

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void setFilter(const LogFilter& filter) override;
    void logsRead(const QList<LogEntry>& entries) override;
    void logAdded(const LogEntry& entry) override;
    void exportToExcel(QXlsx::Document& doc) const override;

private:
    void sortEntries();
    void reverseEntries();
    int  indexOfSortedInsert(QList<LogEntry>* entries, const LogEntry& entry);
    void insertRow(QList<LogEntry>* entries, int row, const LogEntry& entry);
    void filterAll();

    QStringList                      mHeader;
    LogFilter                        mFilter;
    std::shared_ptr<QList<LogEntry>> mEntriesUnfiltered;
    std::shared_ptr<QList<LogEntry>> mEntries;
    int                              mSortColumn;
    Qt::SortOrder                    mSortOrder;
};
