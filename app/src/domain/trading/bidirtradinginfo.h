#pragma once



#include <QMap>

#include "src/domain/stock/stock.h"



enum BiDirMode : quint8
{
    BIDIR_MODE_NONE,
    BIDIR_MODE_HUGE_BID,
    BIDIR_MODE_HUGE_SPREAD
};



struct BiDirTradingInfo
{
    explicit BiDirTradingInfo();
    explicit BiDirTradingInfo(Stock* _stock, BiDirMode _mode, const QString& _cause);
    BiDirTradingInfo(const BiDirTradingInfo& another) = default;
    ~BiDirTradingInfo()                               = default;

    BiDirTradingInfo& operator=(const BiDirTradingInfo& another) = default;

    Stock*    stock;
    BiDirMode mode;
    QString   cause;
};

using InstrumentsForBiDirTrading = QMap<QString, BiDirTradingInfo>; // Instrument UID => BiDirTradingInfo
