#pragma once



#include <QMap>



struct BiDirTradingInfo
{
    explicit BiDirTradingInfo();
    explicit BiDirTradingInfo(const QString& _cause);
    BiDirTradingInfo(const BiDirTradingInfo& another) = default;
    ~BiDirTradingInfo()                               = default;

    BiDirTradingInfo& operator=(const BiDirTradingInfo& another) = default;

    QString cause;
};

using InstrumentsForBiDirTrading = QMap<QString, BiDirTradingInfo>; // Instrument UID => BiDirTradingInfo
