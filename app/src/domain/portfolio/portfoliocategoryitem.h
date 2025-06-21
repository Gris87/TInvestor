#pragma once



#include "src/domain/portfolio/portfolioitem.h"



struct PortfolioCategoryItem
{
    PortfolioCategoryItem();
    PortfolioCategoryItem(const PortfolioCategoryItem& another) = default;
    ~PortfolioCategoryItem()                                    = default;

    PortfolioCategoryItem& operator=(const PortfolioCategoryItem& another) = default;

    int                  id;
    QString              name;
    double               cost;
    float                part;
    QList<PortfolioItem> items;
};



bool operator==(const PortfolioCategoryItem& lhs, const PortfolioCategoryItem& rhs);
