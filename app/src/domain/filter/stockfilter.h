#pragma once



#include "src/domain/stock/stocktableentry.h"



enum QualInvestorFilter : quint8
{
    QUAL_INVESTOR_SHOW_ALL,
    QUAL_INVESTOR_ONLY_WITH_STATUS,
    QUAL_INVESTOR_ONLY_WITHOUT_STATUS
};



struct StockFilter
{
    StockFilter();
    StockFilter(const StockFilter& another) = default;
    ~StockFilter()                          = default;

    StockFilter& operator=(const StockFilter& another) = default;

    [[nodiscard]]
    bool isActive() const;

    [[nodiscard]]
    bool isFiltered(const StockTableEntry& entry) const;

    bool               useTicker;
    QString            ticker;
    bool               useQualInvestor;
    QualInvestorFilter qualInvestor;
    bool               usePrice;
    float              priceFrom;
    float              priceTo;
    bool               useDayStartChange;
    float              dayStartChangeFrom;
    float              dayStartChangeTo;
    bool               useDateChange;
    float              dateChangeFrom;
    float              dateChangeTo;
    bool               useTurnover;
    qint64             turnoverFrom;
    qint64             turnoverTo;
    bool               usePayback;
    float              paybackFrom;
    float              paybackTo;
};



bool operator==(const StockFilter& lhs, const StockFilter& rhs);
