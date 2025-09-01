#pragma once



class ITradeUtils
{
public:
    ITradeUtils()          = default;
    virtual ~ITradeUtils() = default;

    ITradeUtils(const ITradeUtils& another)            = delete;
    ITradeUtils& operator=(const ITradeUtils& another) = delete;
};
