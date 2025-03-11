#pragma once



#include <Qt>



#pragma pack(push, 1)
struct StockData
{
    qint64 timestamp;
    float  price;
};
#pragma pack(pop)
