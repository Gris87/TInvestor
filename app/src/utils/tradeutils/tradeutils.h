#pragma once



#include "src/utils/tradeutils/itradeutils.h"

#include "src/utils/timeutils/itimeutils.h"



class TradeUtils : public ITradeUtils
{
public:
    explicit TradeUtils(ITimeUtils* timeUtils);
    ~TradeUtils() override;

    TradeUtils(const TradeUtils& another)            = delete;
    TradeUtils& operator=(const TradeUtils& another) = delete;

    [[nodiscard]]
    qint64 calculateAmountOfLotsToBuy(
        IConfig* config,
        qint64   timestamp,
        double   money,
        double   totalCost,
        double   turnover,
        double   lotPrice,
        double   lotPriceWithCommission
    ) const override;

    [[nodiscard]]
    qint64 calculateAmountOfLotsToBuy(
        bool   limitStockPurchase,
        double limitStockPurchasePart,
        bool   limitByTurnover,
        double limitByTurnoverPercent,
        double money,
        double totalCost,
        double turnover,
        double lotPrice,
        double lotPriceWithCommission
    ) const override;

private:
    ITimeUtils* mTimeUtils;
};
