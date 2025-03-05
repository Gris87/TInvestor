#pragma once



#include "src/storage/stocks/istocksstorage.h"

#include <gmock/gmock.h>



class StocksStorageMock : public IStocksStorage
{
public:
    StocksStorageMock() :
        IStocksStorage()
    {
    }
    ~StocksStorageMock() override = default;

    StocksStorageMock(const StocksStorageMock& another)            = delete;
    StocksStorageMock& operator=(const StocksStorageMock& another) = delete;

    MOCK_METHOD(void, readFromDatabase, (), (override));
    MOCK_METHOD(QMutex*, getMutex, (), (override));
    MOCK_METHOD(QList<Stock*>&, getStocks, (), (override));
    MOCK_METHOD(bool, mergeStocksMeta, (const QList<StockMeta>& stocksMeta), (override));
    MOCK_METHOD(void, appendStockData, (Stock * stock, const StockData* dataArray, int dataArraySize), (override));
    MOCK_METHOD(void, deleteObsoleteData, (qint64 obsoleteTimestamp, QList<Stock*>& stocks), (override));
};
