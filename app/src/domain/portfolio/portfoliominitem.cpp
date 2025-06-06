#include "src/domain/portfolio/portfoliominitem.h"



PortfolioMinItem::PortfolioMinItem() :
    price(),
    cost()
{
}

bool operator==(const PortfolioMinItem& lhs, const PortfolioMinItem& rhs)
{
    return lhs.price == rhs.price && lhs.cost == rhs.cost;
}
