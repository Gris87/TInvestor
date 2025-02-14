#pragma once



#include <QString>



#pragma pack(push, 1)
struct StockData
{
    qint64 timestamp;
    float  value;
};
#pragma pack(pop)

struct Stock
{
    QString          name;
    QString          fullname;
    QList<StockData> data;
};
