#include "src/storage/instruments/instrumentsstorage.h"

#include <QDebug>



InstrumentsStorage::InstrumentsStorage(IInstrumentsDatabase* instrumentsDatabase) :
    IInstrumentsStorage(),
    mInstrumentsDatabase(instrumentsDatabase),
    mRwMutex(new QReadWriteLock()),
    mInstruments()
{
    qDebug() << "Create InstrumentsStorage";
}

InstrumentsStorage::~InstrumentsStorage()
{
    qDebug() << "Destroy InstrumentsStorage";

    delete mRwMutex;
}

void InstrumentsStorage::readFromDatabase()
{
    mInstruments = mInstrumentsDatabase->readInstruments();
}

void InstrumentsStorage::readLock()
{
    mRwMutex->lockForRead();
}

void InstrumentsStorage::readUnlock()
{
    mRwMutex->unlock();
}

void InstrumentsStorage::writeLock()
{
    mRwMutex->lockForWrite();
}

void InstrumentsStorage::writeUnlock()
{
    mRwMutex->unlock();
}

const Instruments& InstrumentsStorage::getInstruments()
{
    return mInstruments;
}

void InstrumentsStorage::mergeInstruments(const Instruments& instruments)
{
    bool changed = false;

    for (auto it = mInstruments.constBegin(), end = mInstruments.constEnd(); it != end; ++it)
    {
        if (instruments.contains(it.key()))
        {
            const Instrument& instrument = instruments[it.key()];

            if (it.value() != instrument)
            {
                changed = true;

                mInstruments[it.key()] = instrument;
            }
        }
    }

    for (auto it = instruments.constBegin(), end = instruments.constEnd(); it != end; ++it)
    {
        if (!mInstruments.contains(it.key()))
        {
            changed = true;

            mInstruments[it.key()] = it.value();
        }
    }

    if (changed)
    {
        mInstrumentsDatabase->writeInstruments(mInstruments);
    }
}
