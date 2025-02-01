#pragma once



#include "src/db/stocks/istocksdatabase.h"

#include <gmock/gmock.h>



class StocksDatabaseMock : public IStocksDatabase
{
public:
    explicit StocksDatabaseMock() : IStocksDatabase() {}
    ~StocksDatabaseMock() override = default;

    StocksDatabaseMock(const StocksDatabaseMock &another) = delete;
    StocksDatabaseMock& operator=(const StocksDatabaseMock &another) = delete;

    MOCK_METHOD1(createStockTable, void(const QString &name));
    MOCK_METHOD0(readStocks, QList<Stock>());
    MOCK_METHOD1(readStocksData, void(QList<Stock> &stocks));
    MOCK_METHOD2(deleteObsoleteData, void(qint64 obsoleteTimestamp, const QList<Stock> &stocks));
};
