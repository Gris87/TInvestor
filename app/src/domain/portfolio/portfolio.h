#pragma once



#include <QMap>

#include "src/domain/portfolio/portfoliocategoryitem.h"



struct Portfolio
{
    Portfolio();
    Portfolio(const Portfolio& another) = default;
    ~Portfolio()                        = default;

    Portfolio& operator=(const Portfolio& another) = default;

    QMap<QString, PortfolioItems> positionsMap; // Instrument type => positions // TODO: Delete it
    QList<PortfolioCategoryItem>  positionsList;
};



bool operator==(const Portfolio& lhs, const Portfolio& rhs);
