#pragma once



#include <QPixmap>



struct StockTableEntry
{
    StockTableEntry();
    StockTableEntry(const StockTableEntry& another) = default;
    ~StockTableEntry()                              = default;

    StockTableEntry& operator=(const StockTableEntry& another) = default;

    QString  instrumentId;
    QPixmap* instrumentLogo;
    QString  instrumentTicker;
    QString  instrumentName;
    bool     forQualInvestorFlag;
    float    price;
    float    dayChange;
    float    dateChange;
    qint64   turnover;
    float    payback;
    float    dayStartPrice;
    float    specifiedDatePrice;
    qint8    pricePrecision;
};



bool operator==(const StockTableEntry& lhs, const StockTableEntry& rhs);
