#include "src/domain/portfolio/portfolioitem.h"



PortfolioItem::PortfolioItem() :
    showPrices(),
    available(),
    price(),
    avgPrice(),
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
    return lhs.showPrices == rhs.showPrices && lhs.available == rhs.available && lhs.price == rhs.price &&
           lhs.avgPrice == rhs.avgPrice && lhs.cost == rhs.cost && lhs.part == rhs.part && lhs.yield == rhs.yield &&
           lhs.yieldPercent == rhs.yieldPercent && lhs.dailyYield == rhs.dailyYield &&
           lhs.priceForDailyYield == rhs.priceForDailyYield && lhs.costForDailyYield == rhs.costForDailyYield &&
           lhs.dailyYieldPercent == rhs.dailyYieldPercent;
}
