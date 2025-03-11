#pragma once



#include <QList>

#include "src/domain/stocks/stockdata.h"



struct StockOperational
{
    StockOperational();
    StockOperational(const StockOperational& another);
    ~StockOperational();

    StockOperational& operator=(const StockOperational& another);

    qint64           lastStoredTimestamp;
    float            dayStartPrice;
    float            specifiedDatePrice;
    float            payback;
    QList<StockData> detailedData;
};
