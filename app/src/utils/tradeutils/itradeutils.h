#pragma once



#include "src/config/iconfig.h"



class ITradeUtils
{
public:
    ITradeUtils()          = default;
    virtual ~ITradeUtils() = default;

    ITradeUtils(const ITradeUtils& another)            = delete;
    ITradeUtils& operator=(const ITradeUtils& another) = delete;

    [[nodiscard]]
    virtual qint64 calculateAmountOfLotsToBuy(
        IConfig* config, double money, double totalCost, double turnover, double lotPrice, double lotPriceWithCommission
    ) const = 0;
};
