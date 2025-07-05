#include "src/domain/portfolio/portfoliominitem.h"



constexpr float  FLOAT_EPSILON  = 0.0001f;
constexpr double DOUBLE_EPSILON = 0.0001;



PortfolioMinItem::PortfolioMinItem() :
    price(),
    cost()
{
}

bool operator==(const PortfolioMinItem& lhs, const PortfolioMinItem& rhs)
{
    return qAbs(lhs.price - rhs.price) < FLOAT_EPSILON && qAbs(lhs.cost - rhs.cost) < DOUBLE_EPSILON;
}
