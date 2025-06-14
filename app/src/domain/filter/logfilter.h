#pragma once



#include "src/domain/log/logentry.h"



struct LogFilter
{
    LogFilter();
    LogFilter(const LogFilter& another) = default;
    ~LogFilter()                        = default;

    LogFilter& operator=(const LogFilter& another) = default;

    [[nodiscard]]
    bool isActive() const;

    [[nodiscard]]
    bool isFiltered(const LogEntry& entry) const;

    LogLevel level;
    QString  ticker;
};



bool operator==(const LogFilter& lhs, const LogFilter& rhs);
