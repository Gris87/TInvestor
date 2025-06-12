#pragma once



#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel.h"

#include <gmock/gmock.h>



class LogsTableModelMock : public ILogsTableModel
{
    Q_OBJECT

public:
    explicit LogsTableModelMock(QObject* parent = nullptr) :
        ILogsTableModel(parent)
    {
    }
    ~LogsTableModelMock() override = default;

    LogsTableModelMock(const LogsTableModelMock& another)            = delete;
    LogsTableModelMock& operator=(const LogsTableModelMock& another) = delete;

    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(int, columnCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(QVariant, data, (const QModelIndex& index, int role), (const, override));

    MOCK_METHOD(void, logsRead, (const QList<LogEntry>& entries), (override));
    MOCK_METHOD(void, logAdded, (const LogEntry& entry), (override));
};
