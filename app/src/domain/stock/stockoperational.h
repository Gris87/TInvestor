#pragma once



#include <QList>

#include "src/domain/stock/stockdata.h"



struct StockOperational
{
    StockOperational();
    StockOperational(const StockOperational& another) = default;
    ~StockOperational()                               = default;

    StockOperational& operator=(const StockOperational& another) = default;

    qint64                      lastStoredTimestamp;
    float                       dayStartPrice;
    float                       specifiedDatePrice;
    qint64                      turnover;
    float                       payback;
    QList<StockOperationalData> detailedData;
};
