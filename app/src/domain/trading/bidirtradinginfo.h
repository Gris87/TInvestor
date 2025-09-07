#pragma once



#include <QMap>



struct BiDirTradingInfo
{
    explicit BiDirTradingInfo();
    explicit BiDirTradingInfo(qint64 _turnover, const QString& _cause);
    BiDirTradingInfo(const BiDirTradingInfo& another) = default;
    ~BiDirTradingInfo()                               = default;

    BiDirTradingInfo& operator=(const BiDirTradingInfo& another) = default;

    qint64  turnover;
    QString cause;
};

using InstrumentsForBiDirTrading = QMap<QString, BiDirTradingInfo>; // Instrument UID => BiDirTradingInfo
