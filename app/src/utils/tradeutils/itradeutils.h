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
        IConfig* config,
        qint64   timestamp,
        double   money,
        double   totalCost,
        double   turnover,
        double   lotPrice,
        double   lotPriceWithCommission
    ) const = 0;

    [[nodiscard]]
    virtual qint64 calculateAmountOfLotsToBuy(
        bool   limitStockPurchase,
        double limitStockPurchasePart,
        bool   limitByTurnover,
        double limitByTurnoverPercent,
        double money,
        double totalCost,
        double turnover,
        double lotPrice,
        double lotPriceWithCommission
    ) const = 0;
};
