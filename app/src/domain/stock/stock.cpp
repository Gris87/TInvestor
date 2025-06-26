#include "src/domain/stock/stock.h"



Stock::Stock() :
    meta(),
    operational(),
    data(),
    mRwMutex(new QReadWriteLock())
{
}

Stock::Stock(const Stock& another) :
    meta(another.meta),
    operational(another.operational),
    data(another.data),
    mRwMutex(new QReadWriteLock())
{
}

Stock::~Stock()
{
    delete mRwMutex;
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

void Stock::readLock()
{
    mRwMutex->lockForRead();
}

void Stock::readUnlock()
{
    mRwMutex->unlock();
}

void Stock::writeLock()
{
    mRwMutex->lockForWrite();
}

void Stock::writeUnlock()
{
    mRwMutex->unlock();
}

float Stock::lastPrice() const
{
    return !operational.detailedData.isEmpty() ? operational.detailedData.constLast().price
                                               : (!data.isEmpty() ? data.constLast().price : 0);
}
