#pragma once



#include <QString>



struct TradingInfo
{
    TradingInfo();
    TradingInfo(double _expectedCost, const QString& _cause);
    TradingInfo(const TradingInfo& another) = default;
    ~TradingInfo()                          = default;

    TradingInfo& operator=(const TradingInfo& another) = default;

    double  expectedCost;
    QString cause;
};
