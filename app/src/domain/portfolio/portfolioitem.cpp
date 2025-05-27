#include "src/domain/portfolio/portfolioitem.h"



PortfolioItem::PortfolioItem() :
    quantity(),
    price(),
    avgPrice()
{
}

bool operator==(const PortfolioItem& lhs, const PortfolioItem& rhs)
{
    return lhs.quantity == rhs.quantity && lhs.price == rhs.price && lhs.avgPrice == rhs.avgPrice;
}
