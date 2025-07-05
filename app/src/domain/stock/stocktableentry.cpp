#include "src/domain/stock/stocktableentry.h"



constexpr float FLOAT_EPSILON = 0.0001f;



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
           lhs.locked == rhs.locked && qAbs(lhs.price - rhs.price) < FLOAT_EPSILON &&
           qAbs(lhs.dayChange - rhs.dayChange) < FLOAT_EPSILON && qAbs(lhs.dateChange - rhs.dateChange) < FLOAT_EPSILON &&
           lhs.turnover == rhs.turnover && qAbs(lhs.payback - rhs.payback) < FLOAT_EPSILON &&
           qAbs(lhs.dayStartPrice - rhs.dayStartPrice) < FLOAT_EPSILON &&
           qAbs(lhs.specifiedDatePrice - rhs.specifiedDatePrice) < FLOAT_EPSILON && lhs.pricePrecision == rhs.pricePrecision;
}
