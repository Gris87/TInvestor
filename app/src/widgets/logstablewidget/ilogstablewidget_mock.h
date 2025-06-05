#pragma once



#include "src/widgets/logstablewidget/ilogstablewidget.h"

#include <gmock/gmock.h>



class LogsTableWidgetMock : public ILogsTableWidget
{
    Q_OBJECT

public:
    explicit LogsTableWidgetMock(QWidget* parent = nullptr) :
        ILogsTableWidget(parent)
    {
    }
    ~LogsTableWidgetMock() override = default;

    LogsTableWidgetMock(const LogsTableWidgetMock& another)            = delete;
    LogsTableWidgetMock& operator=(const LogsTableWidgetMock& another) = delete;

    MOCK_METHOD(void, setFilter, (LogLevel level), (override));
    MOCK_METHOD(void, logsRead, (const QList<LogEntry>& entries), (override));
    MOCK_METHOD(void, logAdded, (const LogEntry& entry), (override));

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
