#pragma once



#include "src/utils/tradeutils/itradeutils.h"

#include <gmock/gmock.h>



class TradeUtilsMock : public ITradeUtils
{
public:
    TradeUtilsMock() :
        ITradeUtils()
    {
    }
    ~TradeUtilsMock() override = default;

    TradeUtilsMock(const TradeUtilsMock& another)            = delete;
    TradeUtilsMock& operator=(const TradeUtilsMock& another) = delete;

    MOCK_METHOD(
        qint64,
        calculateAmountOfLotsToBuy,
        (IConfig * config,
         qint64 timestamp,
         double money,
         double totalCost,
         double turnover,
         double lotPrice,
         double lotPriceWithCommission),
        (const, override)
    );

    MOCK_METHOD(
        qint64,
        calculateAmountOfLotsToBuy,
        (bool   limitStockPurchase,
         double limitStockPurchasePart,
         bool   limitByTurnover,
         double limitByTurnoverPercent,
         bool   confirmMarginTrade,
         double money,
         double totalCost,
         double turnover,
         double lotPrice,
         double lotPriceWithCommission),
        (const, override)
    );
};
