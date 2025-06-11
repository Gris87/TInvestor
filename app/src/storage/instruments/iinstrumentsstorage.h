#pragma once



#include <QMutex>

#include "src/domain/instrument/instrument.h"



class IInstrumentsStorage
{
public:
    IInstrumentsStorage()          = default;
    virtual ~IInstrumentsStorage() = default;

    IInstrumentsStorage(const IInstrumentsStorage& another)            = delete;
    IInstrumentsStorage& operator=(const IInstrumentsStorage& another) = delete;

    virtual void               readFromDatabase()                               = 0;
    virtual QMutex*            getMutex()                                       = 0;
    virtual void               lock()                                           = 0;
    virtual void               unlock()                                         = 0;
    virtual const Instruments& getInstruments()                                 = 0;
    virtual void               mergeInstruments(const Instruments& instruments) = 0;
};
