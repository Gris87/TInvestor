#include "src/storage/bidirinfos/bidirinfosstorage.h"

#include <QCoreApplication>
#include <QDebug>



BiDirInfosStorage::BiDirInfosStorage(IBiDirInfosDatabase* biDirInfosDatabase, IFileFactory* fileFactory) :
    IBiDirInfosStorage(),
    mRwMutex(new QReadWriteLock()),
    mBiDirInfosDatabase(biDirInfosDatabase),
    mFileFactory(fileFactory),
    mBiDirInfos(),
    mLastModified(-1)
{
    qDebug() << "Create BiDirInfosStorage";
}

BiDirInfosStorage::~BiDirInfosStorage()
{
    qDebug() << "Destroy BiDirInfosStorage";

    delete mRwMutex;
}

void BiDirInfosStorage::readFromDatabase()
{
    std::shared_ptr<IFile> biDirInfoFile =
        mFileFactory->newInstance(qApp->applicationDirPath() + "/data/bidirinfo/bidirinfo.json");

    const qint64 lastModified = biDirInfoFile->lastModified();

    if (mLastModified != lastModified)
    {
        mLastModified = lastModified;

        if (mLastModified == 0)
        {
            biDirInfoFile = mFileFactory->newInstance(":/assets/bidir_info.json");
        }

        mBiDirInfos = mBiDirInfosDatabase->readBiDirInfos(biDirInfoFile);
    }
}

void BiDirInfosStorage::readLock()
{
    mRwMutex->lockForRead();
}

void BiDirInfosStorage::readUnlock()
{
    mRwMutex->unlock();
}

void BiDirInfosStorage::writeLock()
{
    mRwMutex->lockForWrite();
}

void BiDirInfosStorage::writeUnlock()
{
    mRwMutex->unlock();
}

const BiDirInfos& BiDirInfosStorage::getBiDirInfos()
{
    return mBiDirInfos;
}
