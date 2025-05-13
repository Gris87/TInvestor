#pragma once



#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecord.h"

#include <QTableWidget>

#include "src/widgets/tableitems/timetableitem.h"



class OperationsTableRecord : public IOperationsTableRecord
{
    Q_OBJECT

public:
    explicit OperationsTableRecord(QTableWidget* tableWidget, QObject* parent = nullptr);
    ~OperationsTableRecord() override;

    OperationsTableRecord(const OperationsTableRecord& another)            = delete;
    OperationsTableRecord& operator=(const OperationsTableRecord& another) = delete;

    void setOperation(const Operation& operation) override;

private:
    TimeTableItem* mTimeTableWidgetItem;
};
