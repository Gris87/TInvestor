#include "src/storage/instruments/instrumentsstorage.h"

#include <QDebug>



InstrumentsStorage::InstrumentsStorage(IInstrumentsDatabase* instrumentsDatabase) :
    IInstrumentsStorage(),
    mInstrumentsDatabase(instrumentsDatabase),
    mMutex(new QMutex()),
    mInstruments()
{
    qDebug() << "Create InstrumentsStorage";
}

InstrumentsStorage::~InstrumentsStorage()
{
    qDebug() << "Destroy InstrumentsStorage";

    delete mMutex;
}

void InstrumentsStorage::readFromDatabase()
{
    mInstruments = mInstrumentsDatabase->readInstruments();
}

QMutex* InstrumentsStorage::getMutex()
{
    return mMutex;
}

const Instruments& InstrumentsStorage::getInstruments()
{
    return mInstruments;
}

void InstrumentsStorage::mergeInstruments(const Instruments& /*instruments*/)
{
}
