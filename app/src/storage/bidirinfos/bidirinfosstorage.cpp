#include "src/storage/bidirinfos/bidirinfosstorage.h"

#include <QCoreApplication>
#include <QDebug>



BidirInfosStorage::BidirInfosStorage(IBidirInfosDatabase* bidirInfosDatabase, IFileFactory* fileFactory) :
    IBidirInfosStorage(),
    mRwMutex(new QReadWriteLock()),
    mBidirInfosDatabase(bidirInfosDatabase),
    mFileFactory(fileFactory),
    mBidirInfos(),
    mLastModified(-1)
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
    std::shared_ptr<IFile> bidirInfoFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/bidirinfo/bidirinfo.json");

    qint64 lastModified = bidirInfoFile->lastModified();

    if (mLastModified != lastModified)
    {
        mLastModified = lastModified;

        if (mLastModified == 0)
        {
            bidirInfoFile = mFileFactory->newInstance(":/assets/bidir_info.json");
        }

        mBidirInfos = mBidirInfosDatabase->readBidirInfos(bidirInfoFile);
    }
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
