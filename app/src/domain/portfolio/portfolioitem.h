#pragma once



#include <QString>



struct PortfolioItem
{
    PortfolioItem();
    PortfolioItem(const PortfolioItem& another) = default;
    ~PortfolioItem()                            = default;

    PortfolioItem& operator=(const PortfolioItem& another) = default;

    bool   showPrices;
    double available;
    float  price;
    float  avgPriceFifo;
    float  avgPriceWavg;
    double cost;
    float  part;
    float  yield;
    float  yieldPercent;
    float  dailyYield;
    float  priceForDailyYield;
    double costForDailyYield;
    float  dailyYieldPercent;
};

using PortfolioItems = QMap<QString, PortfolioItem>; // UID => PortfolioItem



bool operator==(const PortfolioItem& lhs, const PortfolioItem& rhs);
