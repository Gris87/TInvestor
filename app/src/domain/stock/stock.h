#pragma once



#include <QList>
#include <QReadWriteLock>

#include "src/domain/stock/stockmeta.h"
#include "src/domain/stock/stockoperational.h"



class Stock
{
public:
    Stock();
    Stock(const Stock& another);
    ~Stock();

    Stock& operator=(const Stock& another);

    void readLock();
    void readUnlock();
    void writeLock();
    void writeUnlock();

    [[nodiscard]]
    float lastPrice() const;

    StockMeta        meta;
    StockOperational operational;
    QList<StockData> data;

private:
    QReadWriteLock* mRwMutex;
};
