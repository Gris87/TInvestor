#include "src/domain/trading/tradinginfo.h"



TradingInfo::TradingInfo() :
    asapMode(),
    avgPrice(),
    price(),
    expectedCost(),
    cause()
{
}

TradingInfo::TradingInfo(AsapMode _asapMode, float _avgPrice, float _price, double _expectedCost, const QString& _cause) :
    asapMode(_asapMode),
    avgPrice(_avgPrice),
    price(_price),
    expectedCost(_expectedCost),
    cause(_cause)
{
}
