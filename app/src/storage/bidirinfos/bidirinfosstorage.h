#pragma once



#include "src/storage/bidirinfos/ibidirinfosstorage.h"

#include <QReadWriteLock>

#include "src/db/bidirinfos/ibidirinfosdatabase.h"
#include "src/utils/fs/file/ifilefactory.h"



class BidirInfosStorage : public IBidirInfosStorage
{
public:
    explicit BidirInfosStorage(IBidirInfosDatabase* bidirInfosDatabase, IFileFactory* fileFactory);
    ~BidirInfosStorage() override;

    BidirInfosStorage(const BidirInfosStorage& another)            = delete;
    BidirInfosStorage& operator=(const BidirInfosStorage& another) = delete;

    void               readFromDatabase() override;
    void               readLock() override;
    void               readUnlock() override;
    void               writeLock() override;
    void               writeUnlock() override;
    const BidirInfos&  getBidirInfos() override;

private:
    QReadWriteLock*       mRwMutex;
    IBidirInfosDatabase*  mBidirInfosDatabase;
    IFileFactory*         mFileFactory;
    BidirInfos            mBidirInfos;
    qint64                mLastModified;
};
