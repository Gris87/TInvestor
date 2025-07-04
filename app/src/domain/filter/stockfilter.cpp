#include "src/domain/filter/stockfilter.h"



constexpr qint64 TURNOVER_TO_DEFAULT = 1000000000000;
constexpr float  PAYBACK_TO_DEFAULT  = 100;



StockFilter::StockFilter() :
    useTicker(),
    ticker(),
    useQualInvestor(),
    qualInvestor(),
    usePrice(),
    priceFrom(),
    priceTo(),
    useDayStartChange(),
    dayStartChangeFrom(),
    dayStartChangeTo(),
    useDateChange(),
    dateChangeFrom(),
    dateChangeTo(),
    useTurnover(),
    turnoverFrom(),
    turnoverTo(TURNOVER_TO_DEFAULT),
    usePayback(),
    paybackFrom(),
    paybackTo(PAYBACK_TO_DEFAULT)
{
}

bool StockFilter::isActive() const
{
    return (useTicker && ticker != "") || (useQualInvestor && qualInvestor != QUAL_INVESTOR_SHOW_ALL) || usePrice ||
           useDayStartChange || useDateChange || useTurnover || usePayback;
}

bool StockFilter::isFiltered(const StockTableEntry& entry) const
{
    if (useTicker && ticker != "" && !entry.instrumentTicker.contains(ticker, Qt::CaseInsensitive) &&
        !entry.instrumentName.contains(ticker, Qt::CaseInsensitive))
    {
        return false;
    }

    if (useQualInvestor && qualInvestor != QUAL_INVESTOR_SHOW_ALL &&
        entry.forQualInvestorFlag != (qualInvestor == QUAL_INVESTOR_ONLY_WITH_STATUS))
    {
        return false;
    }

    if (usePrice && (entry.price < priceFrom || entry.price > priceTo))
    {
        return false;
    }

    if (useDayStartChange && (entry.dayChange < dayStartChangeFrom || entry.dayChange > dayStartChangeTo))
    {
        return false;
    }

    if (useDateChange && (entry.dateChange < dateChangeFrom || entry.dateChange > dateChangeTo))
    {
        return false;
    }

    if (useTurnover && (entry.turnover < turnoverFrom || entry.turnover > turnoverTo))
    {
        return false;
    }

    if (usePayback && (entry.payback < paybackFrom || entry.payback > paybackTo))
    {
        return false;
    }

    return true;
}

bool operator==(const StockFilter& lhs, const StockFilter& rhs)
{
    return lhs.useTicker == rhs.useTicker && lhs.ticker == rhs.ticker && lhs.useQualInvestor == rhs.useQualInvestor &&
           lhs.qualInvestor == rhs.qualInvestor && lhs.usePrice == rhs.usePrice &&
           qAbs(lhs.priceFrom - rhs.priceFrom) < 0.0001f && qAbs(lhs.priceTo - rhs.priceTo) < 0.0001f &&
           lhs.useDayStartChange == rhs.useDayStartChange && qAbs(lhs.dayStartChangeFrom - rhs.dayStartChangeFrom) < 0.0001f &&
           qAbs(lhs.dayStartChangeTo - rhs.dayStartChangeTo) < 0.0001f && lhs.useDateChange == rhs.useDateChange &&
           qAbs(lhs.dateChangeFrom - rhs.dateChangeFrom) < 0.0001f && qAbs(lhs.dateChangeTo - rhs.dateChangeTo) < 0.0001f &&
           lhs.useTurnover == rhs.useTurnover && lhs.turnoverFrom == rhs.turnoverFrom && lhs.turnoverTo == rhs.turnoverTo &&
           lhs.usePayback == rhs.usePayback && qAbs(lhs.paybackFrom - rhs.paybackFrom) < 0.0001f &&
           qAbs(lhs.paybackTo - rhs.paybackTo) < 0.0001f;
}
