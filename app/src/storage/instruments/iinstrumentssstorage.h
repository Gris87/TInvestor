#pragma once



#include <QMutex>



class IInstrumentsStorage
{
public:
    IInstrumentsStorage()          = default;
    virtual ~IInstrumentsStorage() = default;

    IInstrumentsStorage(const IInstrumentsStorage& another)            = delete;
    IInstrumentsStorage& operator=(const IInstrumentsStorage& another) = delete;

    virtual void    readFromDatabase() = 0;
    virtual QMutex* getMutex()         = 0;
};
