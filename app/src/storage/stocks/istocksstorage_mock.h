#pragma once



#include "src/storage/stocks/istocksstorage.h"

#include <gmock/gmock.h>



class StocksStorageMock: public IStocksStorage
{
public:
    StocksStorageMock() : IStocksStorage() {}
    ~StocksStorageMock() override = default;

    StocksStorageMock(const StocksStorageMock &another) = delete;
    StocksStorageMock& operator=(const StocksStorageMock &another) = delete;

    MOCK_METHOD0(getStocks, QList<Stock>*());
    MOCK_METHOD1(readFromDatabase, void(IStocksDatabase *stocksDatabase));
};
