#include "src/domain/portfolio/portfolio.h"



Portfolio::Portfolio() :
    positionsMap(),
    positionsList()
{
}

bool operator==(const Portfolio& lhs, const Portfolio& rhs)
{
    return lhs.positionsList == rhs.positionsList;
}
