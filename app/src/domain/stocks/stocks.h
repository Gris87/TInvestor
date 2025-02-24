#pragma once



#include <QList>
#include <QMutex>
#include <QString>

#include "src/domain/quotation/quotation.h"



#pragma pack(push, 1)
struct StockData
{
    qint64 timestamp;
    float  value;
};
#pragma pack(pop)

struct StockMeta
{
    StockMeta();
    StockMeta(const StockMeta& another);
    ~StockMeta();

    StockMeta& operator=(const StockMeta& another);

    void        fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject() const;

    QString uid;
    QString ticker;
    QString name;
    bool    forQualInvestorFlag;
    qint32  lot;
    Quotation minPriceIncrement;
};

struct Stock
{
    Stock();
    Stock(const Stock& another);
    ~Stock();

    Stock& operator=(const Stock& another);

    QMutex*          mutex;
    StockMeta        meta;
    QList<StockData> data;
};



bool operator==(const StockMeta& lhs, const StockMeta& rhs);
