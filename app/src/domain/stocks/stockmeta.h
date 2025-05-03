#pragma once



#include <QString>

#include "src/domain/quotation/quotation.h"



struct StockMeta
{
    StockMeta()                         = default;
    StockMeta(const StockMeta& another) = default;
    ~StockMeta()                        = default;

    StockMeta& operator=(const StockMeta& another) = default;

    void fromJsonObject(const QJsonObject& jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    QString   uid;
    QString   ticker;
    QString   name;
    bool      forQualInvestorFlag;
    qint32    lot;
    Quotation minPriceIncrement;
};



bool operator==(const StockMeta& lhs, const StockMeta& rhs);
