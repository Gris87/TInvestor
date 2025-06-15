#pragma once



#include "src/storage/instruments/iinstrumentsstorage.h"

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
    MOCK_METHOD(void, lock, (), (override));
    MOCK_METHOD(void, unlock, (), (override));
    MOCK_METHOD(const Instruments&, getInstruments, (), (override));
    MOCK_METHOD(void, mergeInstruments, (const Instruments& instruments), (override));
};
