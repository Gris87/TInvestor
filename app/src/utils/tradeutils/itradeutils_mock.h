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
};
