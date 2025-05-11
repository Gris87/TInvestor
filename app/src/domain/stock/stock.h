#pragma once



#include <QList>
#include <QMutex>

#include "src/domain/stock/stockmeta.h"
#include "src/domain/stock/stockoperational.h"



struct Stock
{
    Stock();
    Stock(const Stock& another);
    ~Stock();

    Stock& operator=(const Stock& another);

    [[nodiscard]]
    float lastPrice() const;

    QMutex*          mutex;
    StockMeta        meta;
    StockOperational operational;
    QList<StockData> data;
};
