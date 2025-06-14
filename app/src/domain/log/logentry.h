#pragma once



#include <QJsonObject>



enum LogLevel : quint8
{
    LOG_LEVEL_VERBOSE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_COUNT
};

extern const char* const LOG_LEVEL_NAMES[LOG_LEVEL_COUNT];



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
    QString  instrumentId;
    QPixmap* instrumentLogo;
    QString  instrumentTicker;
    QString  instrumentName;
    QString  message;
};



bool operator==(const LogEntry& lhs, const LogEntry& rhs);
