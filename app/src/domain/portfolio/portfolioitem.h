#pragma once



#include <QString>



struct PortfolioItem
{
    PortfolioItem();
    PortfolioItem(const PortfolioItem& another) = default;
    ~PortfolioItem()                            = default;

    PortfolioItem& operator=(const PortfolioItem& another) = default;

    qint64 quantity;
    float  price;
    float  avgPrice;
};

using PortfolioItems = QMap<QString, PortfolioItem>; // UID => PortfolioItem



bool operator==(const PortfolioItem& lhs, const PortfolioItem& rhs);
