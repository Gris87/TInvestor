#pragma once



#include "src/storage/bidirinfos/ibidirinfosstorage.h"

#include <gmock/gmock.h>



class BidirInfosStorageMock : public IBidirInfosStorage
{
public:
    BidirInfosStorageMock() :
        IBidirInfosStorage()
    {
    }
    ~BidirInfosStorageMock() override = default;

    BidirInfosStorageMock(const BidirInfosStorageMock& another)            = delete;
    BidirInfosStorageMock& operator=(const BidirInfosStorageMock& another) = delete;

    MOCK_METHOD(void, readFromDatabase, (), (override));
    MOCK_METHOD(void, readLock, (), (override));
    MOCK_METHOD(void, readUnlock, (), (override));
    MOCK_METHOD(void, writeLock, (), (override));
    MOCK_METHOD(void, writeUnlock, (), (override));
    MOCK_METHOD(const BidirInfos&, getBidirInfos, (), (override));
};
