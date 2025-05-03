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

    for (auto i = mInstruments.cbegin(), end = mInstruments.cend(); i != end; ++i)
    {
        if (instruments.contains(i.key()))
        {
            const InstrumentInfo& info = instruments[i.key()];

            if (i.value() != info)
            {
                changed = true;

                mInstruments[i.key()] = info;
            }
        }
    }

    for (auto i = instruments.cbegin(), end = instruments.cend(); i != end; ++i)
    {
        if (!mInstruments.contains(i.key()))
        {
            changed = true;

            mInstruments[i.key()] = i.value();
        }
    }

    if (changed)
    {
        mInstrumentsDatabase->writeInstruments(mInstruments);
    }
}
