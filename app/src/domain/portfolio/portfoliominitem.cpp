#include "src/domain/portfolio/portfoliominitem.h"



PortfolioMinItem::PortfolioMinItem() :
    price(),
    cost()
{
}

bool operator==(const PortfolioMinItem& lhs, const PortfolioMinItem& rhs)
{
    return qAbs(lhs.price - rhs.price) < 0.0001f && qAbs(lhs.cost - rhs.cost) < 0.0001;
}
