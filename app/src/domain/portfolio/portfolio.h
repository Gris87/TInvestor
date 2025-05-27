#pragma once



#include <QMap>

#include "src/domain/portfolio/portfolioitem.h"



struct Portfolio
{
    Portfolio();
    Portfolio(const Portfolio& another) = default;
    ~Portfolio()                        = default;

    Portfolio& operator=(const Portfolio& another) = default;

    QMap<QString, PortfolioItems> positions; // Instrument type => positions
};



bool operator==(const Portfolio& lhs, const Portfolio& rhs);
