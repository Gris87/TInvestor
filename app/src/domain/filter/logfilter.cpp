#include "src/domain/filter/logfilter.h"



LogFilter::LogFilter() :
    level(),
    ticker()
{
}

bool LogFilter::isFiltered(LogLevel l, const QString& t, const QString& name) const
{
    if (l < level)
    {
        return false;
    }

    if (ticker != "" && !t.contains(ticker, Qt::CaseInsensitive) && !name.contains(ticker, Qt::CaseInsensitive))
    {
        return false;
    }

    return true;
}

bool operator==(const LogFilter& lhs, const LogFilter& rhs)
{
    return lhs.level == rhs.level && lhs.ticker == rhs.ticker;
}
