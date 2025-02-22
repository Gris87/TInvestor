#pragma once



#include <QList>
#include <QMutex>
#include <QString>



#pragma pack(push, 1)
struct StockData
{
    qint64 timestamp;
    float  value;
};
#pragma pack(pop)

struct Stock
{
    Stock() :
        mutex(new QMutex()),
        uid(),
        ticker(),
        name(),
        forQualInvestorFlag(),
        lot(),
        minPriceIncrement(),
        data()
    {
    }

    Stock(const Stock& another) :
        mutex(new QMutex()),
        uid(another.uid),
        ticker(another.ticker),
        name(another.name),
        forQualInvestorFlag(another.forQualInvestorFlag),
        lot(another.lot),
        minPriceIncrement(another.minPriceIncrement),
        data(another.data)
    {
    }

    ~Stock()
    {
        delete mutex;
    }

    Stock& operator=(const Stock& another)
    {
        uid                 = another.uid;
        ticker              = another.ticker;
        name                = another.name;
        forQualInvestorFlag = another.forQualInvestorFlag;
        lot                 = another.lot;
        minPriceIncrement   = another.minPriceIncrement;
        data                = another.data;

        return *this;
    }

    QMutex*          mutex;
    QString          uid;
    QString          ticker;
    QString          name;
    bool             forQualInvestorFlag;
    int              lot;
    float            minPriceIncrement;
    QList<StockData> data;
};
