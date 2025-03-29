#pragma once



#include <Qt>



#pragma pack(push, 1)
struct StockData
{
    qint64 timestamp;
    qint32 quantity;
    float  price;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct StockOperationalData
{
    qint64 timestamp;
    float  price;
};
#pragma pack(pop)
