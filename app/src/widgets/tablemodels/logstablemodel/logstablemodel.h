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

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
};
