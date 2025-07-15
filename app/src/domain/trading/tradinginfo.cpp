#include "src/domain/trading/tradinginfo.h"



TradingInfo::TradingInfo() :
    price(),
    expectedCost(),
    cause()
{
}

TradingInfo::TradingInfo(float _price, double _expectedCost, const QString& _cause) :
    price(_price),
    expectedCost(_expectedCost),
    cause(_cause)
{
}
