#pragma once



#include "src/widgets/tablerecords/operationstablerecord/ioperationstablerecord.h"

#include <gmock/gmock.h>



class OperationsTableRecordMock : public IOperationsTableRecord
{
    Q_OBJECT

public:
    explicit OperationsTableRecordMock(QObject* parent = nullptr) :
        IOperationsTableRecord(parent)
    {
    }
    ~OperationsTableRecordMock() override = default;

    OperationsTableRecordMock(const OperationsTableRecordMock& another)            = delete;
    OperationsTableRecordMock& operator=(const OperationsTableRecordMock& another) = delete;

    MOCK_METHOD(void, setOperation, (const Operation& operation), (override));
    MOCK_METHOD(void, exportToExcel, (QXlsx::Document & doc), (const, override));
};
