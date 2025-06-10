#include "src/domain/trading/tradinginfo.h"



TradingInfo::TradingInfo() :
    expectedCost(),
    cause()
{
}

TradingInfo::TradingInfo(double _expectedCost, const QString& _cause) :
    expectedCost(_expectedCost),
    cause(_cause)
{
}
