#pragma once



#include <QJsonObject>



enum LogLevel : quint8
{
    LOG_LEVEL_VERBOSE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
};



struct LogEntry
{
    LogEntry();
    LogEntry(const LogEntry& another) = default;
    ~LogEntry()                       = default;

    LogEntry& operator=(const LogEntry& another) = default;

    void fromJsonObject(const QJsonObject& jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64   timestamp;
    LogLevel level;
    QString  message;
};



bool operator==(const LogEntry& lhs, const LogEntry& rhs);
