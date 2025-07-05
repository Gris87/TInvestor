#include "src/domain/portfolio/portfolioitem.h"



constexpr float  FLOAT_EPSILON  = 0.0001f;
constexpr double DOUBLE_EPSILON = 0.0001;



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
           qAbs(lhs.available - rhs.available) < DOUBLE_EPSILON && qAbs(lhs.price - rhs.price) < FLOAT_EPSILON &&
           qAbs(lhs.avgPriceFifo - rhs.avgPriceFifo) < FLOAT_EPSILON && qAbs(lhs.avgPriceWavg - rhs.avgPriceWavg) < FLOAT_EPSILON &&
           qAbs(lhs.cost - rhs.cost) < DOUBLE_EPSILON && qAbs(lhs.part - rhs.part) < FLOAT_EPSILON && qAbs(lhs.yield - rhs.yield) < FLOAT_EPSILON &&
           qAbs(lhs.yieldPercent - rhs.yieldPercent) < FLOAT_EPSILON && qAbs(lhs.dailyYield - rhs.dailyYield) < FLOAT_EPSILON &&
           qAbs(lhs.priceForDailyYield - rhs.priceForDailyYield) < FLOAT_EPSILON &&
           qAbs(lhs.costForDailyYield - rhs.costForDailyYield) < DOUBLE_EPSILON &&
           qAbs(lhs.dailyYieldPercent - rhs.dailyYieldPercent) < FLOAT_EPSILON && lhs.pricePrecision == rhs.pricePrecision;
}
