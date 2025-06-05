#pragma once



#include "src/widgets/tablerecords/logstablerecord/ilogstablerecord.h"

#include <QTableWidget>

#include "src/widgets/tableitems/timetableitem.h"



class LogsTableRecord : public ILogsTableRecord
{
    Q_OBJECT

public:
    explicit LogsTableRecord(QTableWidget* tableWidget, QObject* parent = nullptr);
    ~LogsTableRecord() override;

    LogsTableRecord(const LogsTableRecord& another)            = delete;
    LogsTableRecord& operator=(const LogsTableRecord& another) = delete;

    void setLogEntry(const LogEntry& entry) override;
    void exportToExcel(QXlsx::Document& doc) const override;

private:
    TimeTableItem*    mTimeTableWidgetItem;
    QTableWidgetItem* mLevelTableWidgetItem;
    QTableWidgetItem* mMessageTableWidgetItem;
};
