#ifndef STOCK_H
#define STOCK_H

#include <QString>



struct StockData
{
    qint64 timestamp;
    float  value;
};

struct Stock
{
    QString          name;
    QString          fullname;
    QList<StockData> data;
};



#endif // STOCK_H
