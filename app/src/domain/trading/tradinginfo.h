#pragma once



#include <QMap>



enum AsapMode : quint8
{
    ASAP_MODE_NONE,
    ASAP_MODE_FOLLOW_PRICE,
    ASAP_MODE_IMMEDIATELY_TRADE
};



struct TradingInfo
{
    TradingInfo();
    TradingInfo(AsapMode _asapMode, float _avgPrice, float _price, double _expectedCost, const QString& _cause);
    TradingInfo(const TradingInfo& another) = default;
    ~TradingInfo()                          = default;

    TradingInfo& operator=(const TradingInfo& another) = default;

    AsapMode asapMode;
    float    avgPrice;
    float    price;
    double   expectedCost;
    QString  cause;
};

using InstrumentsForTrading = QMap<QString, TradingInfo>; // Instrument UID => TradingInfo
