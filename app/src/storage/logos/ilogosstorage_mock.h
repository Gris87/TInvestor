#pragma once



#include "src/storage/logos/ilogosstorage.h"

#include <gmock/gmock.h>



class LogosStorageMock : public ILogosStorage
{
public:
    LogosStorageMock() :
        ILogosStorage()
    {
    }
    ~LogosStorageMock() override = default;

    LogosStorageMock(const LogosStorageMock& another)            = delete;
    LogosStorageMock& operator=(const LogosStorageMock& another) = delete;

    MOCK_METHOD(void, readFromDatabase, (), (override));
    MOCK_METHOD(void, readLock, (), (override));
    MOCK_METHOD(void, readUnlock, (), (override));
    MOCK_METHOD(void, writeLock, (), (override));
    MOCK_METHOD(void, writeUnlock, (), (override));
    MOCK_METHOD(void, setLogo, (const QString& instrumentId, const QPixmap& logo), (override));
    MOCK_METHOD(QPixmap*, getLogo, (const QString& instrumentId), (override));
};
