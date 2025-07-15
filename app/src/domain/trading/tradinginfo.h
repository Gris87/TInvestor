#pragma once



#include <QMap>



struct TradingInfo
{
    TradingInfo();
    TradingInfo(float _price, double _expectedCost, const QString& _cause);
    TradingInfo(const TradingInfo& another) = default;
    ~TradingInfo()                          = default;

    TradingInfo& operator=(const TradingInfo& another) = default;

    float   price;
    double  expectedCost;
    QString cause;
};

using InstrumentsForTrading = QMap<QString, TradingInfo>; // Instrument UID => TradingInfo
