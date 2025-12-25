#pragma once



#include "src/domain/bidirinfo/bidirinfo.h"



class IBidirInfosStorage
{
public:
    IBidirInfosStorage()          = default;
    virtual ~IBidirInfosStorage() = default;

    IBidirInfosStorage(const IBidirInfosStorage& another)            = delete;
    IBidirInfosStorage& operator=(const IBidirInfosStorage& another) = delete;

    virtual void              readFromDatabase() = 0;
    virtual void              readLock()         = 0;
    virtual void              readUnlock()       = 0;
    virtual void              writeLock()        = 0;
    virtual void              writeUnlock()      = 0;
    virtual const BidirInfos& getBidirInfos()    = 0;
};
