#pragma once



#include "src/domain/bidirinfo/bidirinfo.h"



class IBiDirInfosStorage
{
public:
    IBiDirInfosStorage()          = default;
    virtual ~IBiDirInfosStorage() = default;

    IBiDirInfosStorage(const IBiDirInfosStorage& another)            = delete;
    IBiDirInfosStorage& operator=(const IBiDirInfosStorage& another) = delete;

    virtual void              readFromDatabase() = 0;
    virtual void              readLock()         = 0;
    virtual void              readUnlock()       = 0;
    virtual void              writeLock()        = 0;
    virtual void              writeUnlock()      = 0;
    virtual const BiDirInfos& getBiDirInfos()    = 0;
};
