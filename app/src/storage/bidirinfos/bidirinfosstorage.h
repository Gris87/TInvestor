#pragma once



#include "src/storage/bidirinfos/ibidirinfosstorage.h"

#include <QReadWriteLock>

#include "src/db/bidirinfos/ibidirinfosdatabase.h"
#include "src/utils/fs/file/ifilefactory.h"



class BiDirInfosStorage : public IBiDirInfosStorage
{
public:
    explicit BiDirInfosStorage(IBiDirInfosDatabase* biDirInfosDatabase, IFileFactory* fileFactory);
    ~BiDirInfosStorage() override;

    BiDirInfosStorage(const BiDirInfosStorage& another)            = delete;
    BiDirInfosStorage& operator=(const BiDirInfosStorage& another) = delete;

    void              readFromDatabase() override;
    void              readLock() override;
    void              readUnlock() override;
    void              writeLock() override;
    void              writeUnlock() override;
    const BiDirInfos& getBiDirInfos() override;

private:
    QReadWriteLock*      mRwMutex;
    IBiDirInfosDatabase* mBiDirInfosDatabase;
    IFileFactory*        mFileFactory;
    BiDirInfos           mBiDirInfos;
    qint64               mLastModified;
};
