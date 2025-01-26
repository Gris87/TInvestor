#pragma once



#include <QString>



struct StockData
{
    qint64 timestamp;
    float  value;
};

struct Stock
{
    QString          name;
    QString          fullname;
    QList<StockData> data;
};
