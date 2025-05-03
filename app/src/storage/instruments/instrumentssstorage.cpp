#include "src/storage/instruments/instrumentssstorage.h"

#include <QDebug>



InstrumentsStorage::InstrumentsStorage() :
    IInstrumentsStorage(),
    mMutex(new QMutex())
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
}

QMutex* InstrumentsStorage::getMutex()
{
    return mMutex;
}
