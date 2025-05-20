#pragma once



#include "src/widgets/tablerecords/stockstablerecord/istockstablerecord.h"

#include <gmock/gmock.h>



class StocksTableRecordMock : public IStocksTableRecord
{
    Q_OBJECT

public:
    explicit StocksTableRecordMock(QObject* parent = nullptr) :
        IStocksTableRecord(parent)
    {
    }
    ~StocksTableRecordMock() override = default;

    StocksTableRecordMock(const StocksTableRecordMock& another)            = delete;
    StocksTableRecordMock& operator=(const StocksTableRecordMock& another) = delete;

    MOCK_METHOD(void, updateAll, (), (override));
    MOCK_METHOD(void, updatePrice, (), (override));
    MOCK_METHOD(void, updatePeriodicData, (), (override));
    MOCK_METHOD(void, filter, (QTableWidget * tableWidget, const Filter& filter), (override));
    MOCK_METHOD(void, exportToExcel, (QXlsx::Document & doc), (const, override));
};
