#include "src/domain/portfolio/portfoliocategoryitem.h"



constexpr float  FLOAT_EPSILON  = 0.0001f;
constexpr double DOUBLE_EPSILON = 0.0001;



PortfolioCategoryItem::PortfolioCategoryItem() :
    id(),
    name(),
    cost(),
    part(),
    items()
{
}

bool operator==(const PortfolioCategoryItem& lhs, const PortfolioCategoryItem& rhs)
{
    return lhs.id == rhs.id && lhs.name == rhs.name && qAbs(lhs.cost - rhs.cost) < DOUBLE_EPSILON &&
           qAbs(lhs.part - rhs.part) < FLOAT_EPSILON && lhs.items == rhs.items;
}
