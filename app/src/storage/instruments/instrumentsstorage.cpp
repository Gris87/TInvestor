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

void InstrumentsStorage::mergeInstruments(const Instruments& instruments)
{
    bool changed = false;

    for (auto it = mInstruments.cbegin(), end = mInstruments.cend(); it != end; ++it)
    {
        if (instruments.contains(it.key()))
        {
            const InstrumentInfo& info = instruments[it.key()];

            if (it.value() != info)
            {
                changed = true;

                mInstruments[it.key()] = info;
            }
        }
    }

    for (auto it = instruments.cbegin(), end = instruments.cend(); it != end; ++it)
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
