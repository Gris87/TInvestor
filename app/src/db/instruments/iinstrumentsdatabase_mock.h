#pragma once



#include "src/db/instruments/iinstrumentsdatabase.h"

#include <gmock/gmock.h>



class InstrumentsDatabaseMock : public IInstrumentsDatabase
{
public:
    explicit InstrumentsDatabaseMock() :
        IInstrumentsDatabase()
    {
    }
    ~InstrumentsDatabaseMock() override = default;

    InstrumentsDatabaseMock(const InstrumentsDatabaseMock& another)            = delete;
    InstrumentsDatabaseMock& operator=(const InstrumentsDatabaseMock& another) = delete;

    MOCK_METHOD(Instruments, readInstruments, (), (override));
    MOCK_METHOD(void, writeInstruments, (const Instruments& instruments), (override));
};
