#include "src/domain/filter/filter.h"



Filter::Filter() :
    useTicker(),
    ticker(),
    usePrice(),
    priceFrom(),
    priceTo(),
    useDayStartChange(),
    dayStartChangeFrom(),
    dayStartChangeTo(),
    useDateChange(),
    dateChangeFrom(),
    dateChangeTo(),
    usePayback(),
    paybackFrom(),
    paybackTo(100)
{
}

Filter::Filter(const Filter& another) :
    useTicker(another.useTicker),
    ticker(another.ticker),
    usePrice(another.usePrice),
    priceFrom(another.priceFrom),
    priceTo(another.priceTo),
    useDayStartChange(another.useDayStartChange),
    dayStartChangeFrom(another.dayStartChangeFrom),
    dayStartChangeTo(another.dayStartChangeTo),
    useDateChange(another.useDateChange),
    dateChangeFrom(another.dateChangeFrom),
    dateChangeTo(another.dateChangeTo),
    usePayback(another.usePayback),
    paybackFrom(another.paybackFrom),
    paybackTo(another.paybackTo)
{
}

Filter::~Filter()
{
}

Filter& Filter::operator=(const Filter& another)
{
    useTicker          = another.useTicker;
    ticker             = another.ticker;
    usePrice           = another.usePrice;
    priceFrom          = another.priceFrom;
    priceTo            = another.priceTo;
    useDayStartChange  = another.useDayStartChange;
    dayStartChangeFrom = another.dayStartChangeFrom;
    dayStartChangeTo   = another.dayStartChangeTo;
    useDateChange      = another.useDateChange;
    dateChangeFrom     = another.dateChangeFrom;
    dateChangeTo       = another.dateChangeTo;
    usePayback         = another.usePayback;
    paybackFrom        = another.paybackFrom;
    paybackTo          = another.paybackTo;

    return *this;
}

bool Filter::isFiltered(const QString& t, float price, float dayStartChange, float dateChange, float payback) const
{
    if (useTicker && ticker != "")
    {
        if (!t.contains(ticker, Qt::CaseInsensitive))
        {
            return false;
        }
    }

    if (usePrice)
    {
        if (price < priceFrom || price > priceTo)
        {
            return false;
        }
    }

    if (useDayStartChange)
    {
        if (dayStartChange < dayStartChangeFrom || dayStartChange > dayStartChangeTo)
        {
            return false;
        }
    }

    if (useDateChange)
    {
        if (dateChange < dateChangeFrom || dateChange > dateChangeTo)
        {
            return false;
        }
    }

    if (usePayback)
    {
        if (payback < paybackFrom || payback > paybackTo)
        {
            return false;
        }
    }

    return true;
}

bool operator==(const Filter& lhs, const Filter& rhs)
{
    return lhs.useTicker == rhs.useTicker && lhs.ticker == rhs.ticker && lhs.usePrice == rhs.usePrice &&
           lhs.priceFrom == rhs.priceFrom && lhs.priceTo == rhs.priceTo && lhs.useDayStartChange == rhs.useDayStartChange &&
           lhs.dayStartChangeFrom == rhs.dayStartChangeFrom && lhs.dayStartChangeTo == rhs.dayStartChangeTo &&
           lhs.useDateChange == rhs.useDateChange && lhs.dateChangeFrom == rhs.dateChangeFrom &&
           lhs.dateChangeTo == rhs.dateChangeTo && lhs.usePayback == rhs.usePayback && lhs.paybackFrom == rhs.paybackFrom &&
           lhs.paybackTo == rhs.paybackTo;
}
