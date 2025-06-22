#pragma once



#include <QString>

#include "src/domain/quotation/quotation.h"



struct StockMeta
{
    StockMeta();
    StockMeta(const StockMeta& another) = default;
    ~StockMeta()                        = default;

    StockMeta& operator=(const StockMeta& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    QString   uid;
    QString   ticker;
    QString   name;
    bool      forQualInvestorFlag;
    qint32    lot;
    Quotation minPriceIncrement; // TODO: Do we need it?
};



bool operator==(const StockMeta& lhs, const StockMeta& rhs);
