#pragma once



#include "src/db/instruments/iinstrumentsdatabase.h"

#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class InstrumentsDatabase : public IInstrumentsDatabase
{
public:
    explicit InstrumentsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory);
    ~InstrumentsDatabase() override;

    InstrumentsDatabase(const InstrumentsDatabase& another)            = delete;
    InstrumentsDatabase& operator=(const InstrumentsDatabase& another) = delete;

    Instruments readInstruments() override;
    void        writeInstruments(const Instruments& instruments) override;

private:
    IFileFactory* mFileFactory;
};
