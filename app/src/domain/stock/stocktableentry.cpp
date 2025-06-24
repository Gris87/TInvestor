#include "src/domain/stock/stocktableentry.h"



StockTableEntry::StockTableEntry() :
    instrumentId(),
    instrumentLogo(),
    instrumentTicker(),
    instrumentName(),
    forQualInvestorFlag(),
    price(),
    dayChange(),
    dateChange(),
    turnover(),
    payback(),
    pricePrecision()
{
}

bool operator==(const StockTableEntry& lhs, const StockTableEntry& rhs)
{
    return lhs.instrumentId == rhs.instrumentId && lhs.instrumentTicker == rhs.instrumentTicker &&
           lhs.instrumentName == rhs.instrumentName && lhs.forQualInvestorFlag == rhs.forQualInvestorFlag &&
           lhs.price == rhs.price && lhs.dayChange == rhs.dayChange && lhs.dateChange == rhs.dateChange &&
           lhs.turnover == rhs.turnover && lhs.payback == rhs.payback && lhs.pricePrecision == rhs.pricePrecision;
}
