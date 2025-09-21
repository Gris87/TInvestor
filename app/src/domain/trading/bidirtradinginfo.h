#pragma once



#include <QMap>

#include "src/domain/stock/stock.h"



struct BiDirTradingInfo
{
    explicit BiDirTradingInfo();
    explicit BiDirTradingInfo(Stock* _stock, const QString& _cause);
    BiDirTradingInfo(const BiDirTradingInfo& another) = default;
    ~BiDirTradingInfo()                               = default;

    BiDirTradingInfo& operator=(const BiDirTradingInfo& another) = default;

    Stock*  stock;
    QString cause;
};

using InstrumentsForBiDirTrading = QMap<QString, BiDirTradingInfo>; // Instrument UID => BiDirTradingInfo
