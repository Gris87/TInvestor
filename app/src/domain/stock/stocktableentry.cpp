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
           lhs.locked == rhs.locked && qAbs(lhs.price - rhs.price) < 0.0001f && qAbs(lhs.dayChange - rhs.dayChange) < 0.0001f &&
           qAbs(lhs.dateChange - rhs.dateChange) < 0.0001f && lhs.turnover == rhs.turnover &&
           qAbs(lhs.payback - rhs.payback) < 0.0001f && qAbs(lhs.dayStartPrice - rhs.dayStartPrice) < 0.0001f &&
           qAbs(lhs.specifiedDatePrice - rhs.specifiedDatePrice) < 0.0001f && lhs.pricePrecision == rhs.pricePrecision;
}
