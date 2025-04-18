#pragma once



#include <QList>
#include <QMutex>

#include "src/domain/stocks/stockmeta.h"
#include "src/domain/stocks/stockoperational.h"



struct Stock
{
    Stock();
    Stock(const Stock& another);
    ~Stock();

    Stock& operator=(const Stock& another);

    QMutex*          mutex;
    StockMeta        meta;
    StockOperational operational;
    QList<StockData> data;
};
