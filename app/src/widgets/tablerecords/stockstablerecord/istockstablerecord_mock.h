#pragma once



#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord.h"

#include <gmock/gmock.h>



class TableRecordMock : public ITableRecord
{
public:
    explicit TableRecordMock(QObject* parent = nullptr) :
        ITableRecord(parent)
    {
    }
    ~TableRecordMock() override = default;

    TableRecordMock(const TableRecordMock& another)            = delete;
    TableRecordMock& operator=(const TableRecordMock& another) = delete;

    MOCK_METHOD(void, updateAll, (), (override));
    MOCK_METHOD(void, updatePrice, (), (override));
    MOCK_METHOD(void, updatePeriodicData, (), (override));
    MOCK_METHOD(void, filter, (QTableWidget * tableWidget, const Filter& filter), (override));
};
