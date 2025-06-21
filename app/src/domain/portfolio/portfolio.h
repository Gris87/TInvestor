#pragma once



#include "src/domain/portfolio/portfoliocategoryitem.h"



struct Portfolio
{
    Portfolio();
    Portfolio(const Portfolio& another) = default;
    ~Portfolio()                        = default;

    Portfolio& operator=(const Portfolio& another) = default;

    QList<PortfolioCategoryItem> positions;
};



bool operator==(const Portfolio& lhs, const Portfolio& rhs);
