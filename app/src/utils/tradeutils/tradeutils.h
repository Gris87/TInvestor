#pragma once



#include "src/utils/tradeutils/itradeutils.h"



class TradeUtils : public ITradeUtils
{
public:
    TradeUtils();
    ~TradeUtils() override;

    TradeUtils(const TradeUtils& another)            = delete;
    TradeUtils& operator=(const TradeUtils& another) = delete;

    [[nodiscard]]
    qint64 calculateAmountOfLotsToBuy(
        IConfig* config, double money, double totalCost, double turnover, double lotPrice, double lotPriceWithCommission
    ) const override;
};
