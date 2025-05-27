#include "src/domain/portfolio/portfolio.h"



Portfolio::Portfolio() :
    positions()
{
}

bool operator==(const Portfolio& lhs, const Portfolio& rhs)
{
    return lhs.positions == rhs.positions;
}
