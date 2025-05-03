#pragma once



#include "src/db/instruments/iinstrumentsdatabase.h"



class InstrumentsDatabase : public IInstrumentsDatabase
{
public:
    explicit InstrumentsDatabase();
    ~InstrumentsDatabase() override;

    InstrumentsDatabase(const InstrumentsDatabase& another)            = delete;
    InstrumentsDatabase& operator=(const InstrumentsDatabase& another) = delete;
};
