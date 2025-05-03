#pragma once



#include "src/storage/instruments/iinstrumentssstorage.h"

#include <gmock/gmock.h>



class InstrumentsStorageMock : public IInstrumentsStorage
{
public:
    InstrumentsStorageMock() :
        IInstrumentsStorage()
    {
    }
    ~InstrumentsStorageMock() override = default;

    InstrumentsStorageMock(const InstrumentsStorageMock& another)            = delete;
    InstrumentsStorageMock& operator=(const InstrumentsStorageMock& another) = delete;

    MOCK_METHOD(void, readFromDatabase, (), (override));
    MOCK_METHOD(QMutex*, getMutex, (), (override));
};
