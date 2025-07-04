#include "src/domain/portfolio/portfoliocategoryitem.h"



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
    return lhs.id == rhs.id && lhs.name == rhs.name && qAbs(lhs.cost - rhs.cost) < 0.0001 &&
           qAbs(lhs.part - rhs.part) < 0.0001f && lhs.items == rhs.items;
}
