#include "src/domain/portfolio/portfoliocategoryitem.h"



PortfolioCategoryItem::PortfolioCategoryItem() :
    name(),
    cost(),
    part(),
    items()
{
}

bool operator==(const PortfolioCategoryItem& lhs, const PortfolioCategoryItem& rhs)
{
    return lhs.name == rhs.name && lhs.cost == rhs.cost && lhs.part == rhs.part && lhs.items == rhs.items;
}
