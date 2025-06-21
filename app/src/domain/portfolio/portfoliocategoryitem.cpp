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
    return lhs.id == rhs.id && lhs.name == rhs.name && lhs.cost == rhs.cost && lhs.part == rhs.part && lhs.items == rhs.items;
}
