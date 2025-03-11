#pragma once



#include <QString>

#include "src/domain/quotation/quotation.h"



struct StockMeta
{
    StockMeta();
    StockMeta(const StockMeta& another);
    ~StockMeta();

    StockMeta& operator=(const StockMeta& another);

    void        fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject() const;

    QString   uid;
    QString   ticker;
    QString   name;
    bool      forQualInvestorFlag;
    qint32    lot;
    Quotation minPriceIncrement;
};



bool operator==(const StockMeta& lhs, const StockMeta& rhs);
