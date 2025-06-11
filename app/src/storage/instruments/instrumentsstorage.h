#pragma once



#include "src/storage/instruments/iinstrumentsstorage.h"

#include "src/db/instruments/iinstrumentsdatabase.h"



class InstrumentsStorage : public IInstrumentsStorage
{
public:
    explicit InstrumentsStorage(IInstrumentsDatabase* instrumentsDatabase);
    ~InstrumentsStorage() override;

    InstrumentsStorage(const InstrumentsStorage& another)            = delete;
    InstrumentsStorage& operator=(const InstrumentsStorage& another) = delete;

    void               readFromDatabase() override;
    QMutex*            getMutex() override;
    void               lock() override;
    void               unlock() override;
    const Instruments& getInstruments() override;
    void               mergeInstruments(const Instruments& instruments) override;

private:
    IInstrumentsDatabase* mInstrumentsDatabase;
    QMutex*               mMutex;
    Instruments           mInstruments;
};
