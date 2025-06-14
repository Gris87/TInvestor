#include "src/domain/filter/logfilter.h"



LogFilter::LogFilter() :
    level(),
    ticker()
{
}

bool LogFilter::isActive() const
{
    return level != LOG_LEVEL_VERBOSE || ticker != "";
}

bool LogFilter::isFiltered(const LogEntry& entry) const
{
    if (entry.level < level)
    {
        return false;
    }

    if (ticker != "" && !entry.instrumentTicker.contains(ticker, Qt::CaseInsensitive) &&
        !entry.instrumentName.contains(ticker, Qt::CaseInsensitive))
    {
        return false;
    }

    return true;
}

bool operator==(const LogFilter& lhs, const LogFilter& rhs)
{
    return lhs.level == rhs.level && lhs.ticker == rhs.ticker;
}
