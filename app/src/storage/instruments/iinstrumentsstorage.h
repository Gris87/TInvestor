#pragma once



#include "src/domain/instrument/instrument.h"



class IInstrumentsStorage
{
public:
    IInstrumentsStorage()          = default;
    virtual ~IInstrumentsStorage() = default;

    IInstrumentsStorage(const IInstrumentsStorage& another)            = delete;
    IInstrumentsStorage& operator=(const IInstrumentsStorage& another) = delete;

    virtual void               readFromDatabase()                               = 0;
    virtual void               readLock()                                       = 0;
    virtual void               readUnlock()                                     = 0;
    virtual void               writeLock()                                      = 0;
    virtual void               writeUnlock()                                    = 0;
    virtual const Instruments& getInstruments()                                 = 0;
    virtual void               mergeInstruments(const Instruments& instruments) = 0;
};
