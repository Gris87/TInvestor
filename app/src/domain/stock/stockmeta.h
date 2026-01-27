#pragma once



#include <QJsonObject>

#include "src/domain/logo/logo.h"
#include "src/domain/stock/stockdividendsmeta.h"
#include "src/domain/stock/stockshortsmeta.h"
#include "src/simdjson/simdjson_wrapped.h"



struct StockMeta
{
    explicit StockMeta();
    StockMeta(const StockMeta& another) = default;
    ~StockMeta()                        = default;

    StockMeta& operator=(const StockMeta& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    bool compareForMerge(const StockMeta& another);

    QString            instrumentId;
    Logo*              instrumentLogo;
    QString            instrumentTicker;
    QString            instrumentName;
    bool               forQualInvestorFlag;
    qint32             lot;
    qint8              pricePrecision;
    float              minPriceIncrement;
    QTime              lastTradeTime;
    qint64             turnover;
    StockDividendsMeta dividends;
    StockShortsMeta    shorts;
};



bool operator==(const StockMeta& lhs, const StockMeta& rhs);
