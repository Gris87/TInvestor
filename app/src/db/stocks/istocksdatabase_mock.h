#pragma once



#include "src/db/stocks/istocksdatabase.h"

#include <gmock/gmock.h>



class StocksDatabaseMock : public IStocksDatabase
{
public:
    explicit StocksDatabaseMock() :
        IStocksDatabase()
    {
    }
    ~StocksDatabaseMock() override = default;

    StocksDatabaseMock(const StocksDatabaseMock& another)            = delete;
    StocksDatabaseMock& operator=(const StocksDatabaseMock& another) = delete;

    MOCK_METHOD(QList<Stock>, readStocksMeta, (), (override));
    MOCK_METHOD(void, readStocksData, (QList<Stock> * stocks), (override));
    MOCK_METHOD(void, writeStocksMeta, (QList<Stock> * stocks), (override));
    MOCK_METHOD(void, appendStockData, (Stock * stock), (override));
    MOCK_METHOD(void, deleteObsoleteData, (qint64 obsoleteTimestamp, QList<Stock>* stocks), (override));
};
