#pragma once



#include "src/storage/istockstorage.h"

#include <gmock/gmock.h>



class StockStorageMock: public IStockStorage
{
public:
    StockStorageMock() : IStockStorage() {}
    ~StockStorageMock() override = default;

    StockStorageMock(const StockStorageMock &another) = delete;
    StockStorageMock& operator=(const StockStorageMock &another) = delete;
};
