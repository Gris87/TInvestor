#pragma once



#include "src/storage/bidirinfos/ibidirinfosstorage.h"

#include <gmock/gmock.h>



class BiDirInfosStorageMock : public IBiDirInfosStorage
{
public:
    BiDirInfosStorageMock() :
        IBiDirInfosStorage()
    {
    }
    ~BiDirInfosStorageMock() override = default;

    BiDirInfosStorageMock(const BiDirInfosStorageMock& another)            = delete;
    BiDirInfosStorageMock& operator=(const BiDirInfosStorageMock& another) = delete;

    MOCK_METHOD(void, readFromDatabase, (), (override));
    MOCK_METHOD(void, readLock, (), (override));
    MOCK_METHOD(void, readUnlock, (), (override));
    MOCK_METHOD(void, writeLock, (), (override));
    MOCK_METHOD(void, writeUnlock, (), (override));
    MOCK_METHOD(const BiDirInfos&, getBiDirInfos, (), (override));
};
