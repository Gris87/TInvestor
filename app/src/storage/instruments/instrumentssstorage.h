#pragma once



#include "src/storage/instruments/iinstrumentssstorage.h"



class InstrumentsStorage : public IInstrumentsStorage
{
public:
    explicit InstrumentsStorage();
    ~InstrumentsStorage() override;

    InstrumentsStorage(const InstrumentsStorage& another)            = delete;
    InstrumentsStorage& operator=(const InstrumentsStorage& another) = delete;

    void    readFromDatabase() override;
    QMutex* getMutex() override;

private:
    QMutex* mMutex;
};
