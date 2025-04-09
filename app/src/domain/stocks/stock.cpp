#include "src/domain/stocks/stock.h"



Stock::Stock() :
    mutex(new QMutex()),
    meta(),
    operational(),
    data()
{
}

Stock::Stock(const Stock& another) :
    mutex(new QMutex()),
    meta(another.meta),
    operational(another.operational),
    data(another.data)
{
}

Stock::~Stock()
{
    delete mutex;
}

Stock& Stock::operator=(const Stock& another)
{
    if (&another != this)
    {
        meta        = another.meta;
        operational = another.operational;
        data        = another.data;
    }

    return *this;
}
