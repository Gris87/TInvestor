#include "src/domain/portfolio/portfolioitem.h"



PortfolioItem::PortfolioItem() :
    instrumentId(),
    instrumentLogo(),
    instrumentTicker(),
    instrumentName(),
    showPrices(),
    available(),
    price(),
    avgPriceFifo(),
    avgPriceWavg(),
    cost(),
    part(),
    yield(),
    yieldPercent(),
    dailyYield(),
    priceForDailyYield(),
    costForDailyYield(),
    dailyYieldPercent(),
    pricePrecision()
{
}

bool operator==(const PortfolioItem& lhs, const PortfolioItem& rhs)
{
    return lhs.instrumentId == rhs.instrumentId && lhs.instrumentTicker == rhs.instrumentTicker &&
           lhs.instrumentName == rhs.instrumentName && lhs.showPrices == rhs.showPrices &&
           qAbs(lhs.available - rhs.available) < 0.0001 && qAbs(lhs.price - rhs.price) < 0.0001f &&
           qAbs(lhs.avgPriceFifo - rhs.avgPriceFifo) < 0.0001f && qAbs(lhs.avgPriceWavg - rhs.avgPriceWavg) < 0.0001f &&
           qAbs(lhs.cost - rhs.cost) < 0.0001 && qAbs(lhs.part - rhs.part) < 0.0001f && qAbs(lhs.yield - rhs.yield) < 0.0001f &&
           qAbs(lhs.yieldPercent - rhs.yieldPercent) < 0.0001f && qAbs(lhs.dailyYield - rhs.dailyYield) < 0.0001f &&
           qAbs(lhs.priceForDailyYield - rhs.priceForDailyYield) < 0.0001f &&
           qAbs(lhs.costForDailyYield - rhs.costForDailyYield) < 0.0001 &&
           qAbs(lhs.dailyYieldPercent - rhs.dailyYieldPercent) < 0.0001f && lhs.pricePrecision == rhs.pricePrecision;
}
