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
    dailyYieldPercent()
{
}

bool operator==(const PortfolioItem& lhs, const PortfolioItem& rhs)
{
    return lhs.instrumentId == rhs.instrumentId && lhs.instrumentTicker == rhs.instrumentTicker &&
           lhs.instrumentName == rhs.instrumentName && lhs.showPrices == rhs.showPrices && lhs.available == rhs.available &&
           lhs.price == rhs.price && lhs.avgPriceFifo == rhs.avgPriceFifo && lhs.avgPriceWavg == rhs.avgPriceWavg &&
           lhs.cost == rhs.cost && lhs.part == rhs.part && lhs.yield == rhs.yield && lhs.yieldPercent == rhs.yieldPercent &&
           lhs.dailyYield == rhs.dailyYield && lhs.priceForDailyYield == rhs.priceForDailyYield &&
           lhs.costForDailyYield == rhs.costForDailyYield && lhs.dailyYieldPercent == rhs.dailyYieldPercent;
}
