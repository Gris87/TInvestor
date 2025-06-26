#pragma once



#include "src/storage/instruments/iinstrumentsstorage.h"

#include <QReadWriteLock>

#include "src/db/instruments/iinstrumentsdatabase.h"



class InstrumentsStorage : public IInstrumentsStorage
{
public:
    explicit InstrumentsStorage(IInstrumentsDatabase* instrumentsDatabase);
    ~InstrumentsStorage() override;

    InstrumentsStorage(const InstrumentsStorage& another)            = delete;
    InstrumentsStorage& operator=(const InstrumentsStorage& another) = delete;

    void               readFromDatabase() override;
    void               readLock() override;
    void               readUnlock() override;
    void               writeLock() override;
    void               writeUnlock() override;
    const Instruments& getInstruments() override;
    void               mergeInstruments(const Instruments& instruments) override;

private:
    IInstrumentsDatabase* mInstrumentsDatabase;
    QReadWriteLock*       mRwMutex;
    Instruments           mInstruments;
};
