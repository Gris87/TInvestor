#pragma once



#include "src/domain/instruments/instrumentinfo.h"



class IInstrumentsDatabase
{
public:
    explicit IInstrumentsDatabase() = default;
    virtual ~IInstrumentsDatabase() = default;

    IInstrumentsDatabase(const IInstrumentsDatabase& another)            = delete;
    IInstrumentsDatabase& operator=(const IInstrumentsDatabase& another) = delete;

    virtual Instruments readInstruments()                                = 0;
    virtual void        writeInstruments(const Instruments& instruments) = 0;
};
