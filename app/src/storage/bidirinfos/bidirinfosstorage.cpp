#include "src/storage/bidirinfos/bidirinfosstorage.h"

#include <QDebug>



BidirInfosStorage::BidirInfosStorage(IBidirInfosDatabase* bidirInfosDatabase) :
    IBidirInfosStorage(),
    mRwMutex(new QReadWriteLock()),
    mBidirInfosDatabase(bidirInfosDatabase),
    mBidirInfos()
{
    qDebug() << "Create BidirInfosStorage";
}

BidirInfosStorage::~BidirInfosStorage()
{
    qDebug() << "Destroy BidirInfosStorage";

    delete mRwMutex;
}

void BidirInfosStorage::readFromDatabase()
{
    mBidirInfos = mBidirInfosDatabase->readBidirInfos();
}

void BidirInfosStorage::readLock()
{
    mRwMutex->lockForRead();
}

void BidirInfosStorage::readUnlock()
{
    mRwMutex->unlock();
}

void BidirInfosStorage::writeLock()
{
    mRwMutex->lockForWrite();
}

void BidirInfosStorage::writeUnlock()
{
    mRwMutex->unlock();
}

const BidirInfos& BidirInfosStorage::getBidirInfos()
{
    return mBidirInfos;
}
