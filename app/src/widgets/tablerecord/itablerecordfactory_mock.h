#pragma once



#include "src/widgets/tablerecord/itablerecordfactory.h"

#include <gmock/gmock.h>



class TableRecordFactoryMock : public ITableRecordFactory
{
public:
    TableRecordFactoryMock() :
        ITableRecordFactory()
    {
    }
    ~TableRecordFactoryMock() override = default;

    TableRecordFactoryMock(const TableRecordFactoryMock& another)            = delete;
    TableRecordFactoryMock& operator=(const TableRecordFactoryMock& another) = delete;

    MOCK_METHOD(ITableRecord*, newInstance, (QTableWidget * tableWidget, Stock* stock, QObject* parent), (override));
};
