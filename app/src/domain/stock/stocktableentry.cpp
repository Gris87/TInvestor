#include "src/domain/stock/stocktableentry.h"



StockTableEntry::StockTableEntry() :
    instrumentId(),
    instrumentLogo(),
    instrumentTicker(),
    instrumentName(),
    forQualInvestorFlag(),
    locked(),
    price(),
    dayChange(),
    dateChange(),
    turnover(),
    payback(),
    dayStartPrice(),
    specifiedDatePrice(),
    pricePrecision()
{
}

bool operator==(const StockTableEntry& lhs, const StockTableEntry& rhs)
{
    return lhs.instrumentId == rhs.instrumentId && lhs.instrumentTicker == rhs.instrumentTicker &&
           lhs.instrumentName == rhs.instrumentName && lhs.forQualInvestorFlag == rhs.forQualInvestorFlag &&
           lhs.locked == rhs.locked && lhs.price == rhs.price && lhs.dayChange == rhs.dayChange &&
           lhs.dateChange == rhs.dateChange && lhs.turnover == rhs.turnover && lhs.payback == rhs.payback &&
           lhs.dayStartPrice == rhs.dayStartPrice && lhs.specifiedDatePrice == rhs.specifiedDatePrice &&
           lhs.pricePrecision == rhs.pricePrecision;
}
