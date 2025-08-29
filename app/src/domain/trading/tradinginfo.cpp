#include "src/domain/trading/tradinginfo.h"



TradingInfo::TradingInfo() :
    asap(),
    avgPrice(),
    price(),
    expectedCost(),
    cause()
{
}

TradingInfo::TradingInfo(bool _asap, float _avgPrice, float _price, double _expectedCost, const QString& _cause) :
    asap(_asap),
    avgPrice(_avgPrice),
    price(_price),
    expectedCost(_expectedCost),
    cause(_cause)
{
}
