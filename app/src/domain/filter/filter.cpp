#include "src/domain/filter/filter.h"



constexpr qint64 TURNOVER_TO_DEFAULT = 1000000000000;
constexpr float  PAYBACK_TO_DEFAULT  = 100;



Filter::Filter() :
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

bool Filter::isActive() const
{
    return (useTicker && ticker != "") || (useQualInvestor && qualInvestor != QUAL_INVESTOR_SHOW_ALL) || usePrice ||
           useDayStartChange || useDateChange || useTurnover || usePayback;
}

bool Filter::isFiltered(
    const QString& t,
    const QString& name,
    bool           forQualInvestorFlag,
    float          price,
    float          dayStartChange,
    float          dateChange,
    qint64         turnover,
    float          payback
) const
{
    if (useTicker && ticker != "" && !t.contains(ticker, Qt::CaseInsensitive) && !name.contains(ticker, Qt::CaseInsensitive))
    {
        return false;
    }

    if (useQualInvestor && qualInvestor != QUAL_INVESTOR_SHOW_ALL &&
        forQualInvestorFlag != (qualInvestor == QUAL_INVESTOR_ONLY_WITH_STATUS))
    {
        return false;
    }

    if (usePrice && (price < priceFrom || price > priceTo))
    {
        return false;
    }

    if (useDayStartChange && (dayStartChange < dayStartChangeFrom || dayStartChange > dayStartChangeTo))
    {
        return false;
    }

    if (useDateChange && (dateChange < dateChangeFrom || dateChange > dateChangeTo))
    {
        return false;
    }

    if (useTurnover && (turnover < turnoverFrom || turnover > turnoverTo))
    {
        return false;
    }

    if (usePayback && (payback < paybackFrom || payback > paybackTo))
    {
        return false;
    }

    return true;
}

bool operator==(const Filter& lhs, const Filter& rhs)
{
    return lhs.useTicker == rhs.useTicker && lhs.ticker == rhs.ticker && lhs.useQualInvestor == rhs.useQualInvestor &&
           lhs.qualInvestor == rhs.qualInvestor && lhs.usePrice == rhs.usePrice && lhs.priceFrom == rhs.priceFrom &&
           lhs.priceTo == rhs.priceTo && lhs.useDayStartChange == rhs.useDayStartChange &&
           lhs.dayStartChangeFrom == rhs.dayStartChangeFrom && lhs.dayStartChangeTo == rhs.dayStartChangeTo &&
           lhs.useDateChange == rhs.useDateChange && lhs.dateChangeFrom == rhs.dateChangeFrom &&
           lhs.dateChangeTo == rhs.dateChangeTo && lhs.useTurnover == rhs.useTurnover && lhs.turnoverFrom == rhs.turnoverFrom &&
           lhs.turnoverTo == rhs.turnoverTo && lhs.usePayback == rhs.usePayback && lhs.paybackFrom == rhs.paybackFrom &&
           lhs.paybackTo == rhs.paybackTo;
}
