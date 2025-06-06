#pragma once



#include <QString>



struct PortfolioMinItem
{
    PortfolioMinItem();
    PortfolioMinItem(const PortfolioMinItem& another) = default;
    ~PortfolioMinItem()                               = default;

    PortfolioMinItem& operator=(const PortfolioMinItem& another) = default;

    float  price;
    double cost;
};

using PortfolioMinItems = QMap<QString, PortfolioMinItem>; // UID => PortfolioMinItem



bool operator==(const PortfolioMinItem& lhs, const PortfolioMinItem& rhs);
