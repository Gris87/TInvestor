#pragma once



#include "src/domain/log/logentry.h"



struct LogFilter
{
    LogFilter();
    LogFilter(const LogFilter& another) = default;
    ~LogFilter()                        = default;

    LogFilter& operator=(const LogFilter& another) = default;

    [[nodiscard]]
    bool isFiltered(LogLevel l, const QString& t, const QString& name) const;

    LogLevel level;
    QString  ticker;
};



bool operator==(const LogFilter& lhs, const LogFilter& rhs);
