#pragma once



#include <QPixmap>



struct PortfolioItem
{
    PortfolioItem();
    PortfolioItem(const PortfolioItem& another) = default;
    ~PortfolioItem()                            = default;

    PortfolioItem& operator=(const PortfolioItem& another) = default;

    QString  instrumentId;
    QPixmap* instrumentLogo;
    QString  instrumentTicker;
    QString  instrumentName;
    bool     showPrices;
    double   available;
    float    price;
    float    avgPriceFifo;
    float    avgPriceWavg;
    double   cost;
    float    part;
    float    yield;
    float    yieldPercent;
    float    dailyYield;
    float    priceForDailyYield;
    double   costForDailyYield;
    float    dailyYieldPercent;
    qint8    pricePrecision;
};



bool operator==(const PortfolioItem& lhs, const PortfolioItem& rhs);
