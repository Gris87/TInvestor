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

    MOCK_METHOD(QList<Stock*>, readStocksMeta, (), (override));
    MOCK_METHOD(void, readStocksData, (QList<Stock*> & stocks), (override));
    MOCK_METHOD(void, writeStocksMeta, (const QList<Stock*>& stocks), (override));
    MOCK_METHOD(void, appendStockData, (Stock * stock, const StockData* dataArray, int dataArraySize), (override));
    MOCK_METHOD(void, writeStockData, (const Stock& stock), (override));
};
