#pragma once



#include "src/domain/logo/logo.h"
#include "src/domain/stock/stockdividendsmeta.h"
#include "src/domain/stock/stockshortsmeta.h"



struct StockTableEntry
{
    explicit StockTableEntry();
    StockTableEntry(const StockTableEntry& another) = default;
    ~StockTableEntry()                              = default;

    StockTableEntry& operator=(const StockTableEntry& another) = default;

    QString            instrumentId;
    Logo*              instrumentLogo;
    QString            instrumentTicker;
    QString            instrumentName;
    bool               forQualInvestorFlag;
    bool               locked;
    float              price;
    float              dayChange;
    float              dateChange;
    qint64             turnover;
    float              payback;
    StockDividendsMeta dividends;
    StockShortsMeta    shorts;
    float              dayStartPrice;
    float              specifiedDatePrice;
    qint8              pricePrecision;
};



bool operator==(const StockTableEntry& lhs, const StockTableEntry& rhs);
