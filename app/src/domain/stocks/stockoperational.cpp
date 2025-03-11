#include "src/domain/stocks/stockoperational.h"



StockOperational::StockOperational() :
    lastStoredTimestamp(),
    dayStartPrice(-1),
    specifiedDatePrice(-1),
    payback(),
    detailedData()
{
}

StockOperational::StockOperational(const StockOperational& another) :
    lastStoredTimestamp(another.lastStoredTimestamp),
    dayStartPrice(another.dayStartPrice),
    specifiedDatePrice(another.specifiedDatePrice),
    payback(another.payback),
    detailedData(another.detailedData)
{
}

StockOperational::~StockOperational()
{
}

StockOperational& StockOperational::operator=(const StockOperational& another)
{
    lastStoredTimestamp = another.lastStoredTimestamp;
    dayStartPrice       = another.dayStartPrice;
    specifiedDatePrice  = another.specifiedDatePrice;
    payback             = another.payback;
    detailedData        = another.detailedData;

    return *this;
}
