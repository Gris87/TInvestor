#include "src/db/instruments/instrumentsdatabase.h"

#include <QCoreApplication>
#include <QDebug>



InstrumentsDatabase::InstrumentsDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory) :
    IInstrumentsDatabase(),
    mFileFactory(fileFactory)
{
    qDebug() << "Create InstrumentsDatabase";

    const std::shared_ptr<IDir> dir = dirFactory->newInstance();

    const bool ok = dir->mkpath(qApp->applicationDirPath() + "/data/instruments/logos");
    Q_ASSERT_X(ok, "InstrumentsDatabase::InstrumentsDatabase()", "Failed to create dir");
}

InstrumentsDatabase::~InstrumentsDatabase()
{
    qDebug() << "Destroy InstrumentsDatabase";
}

Instruments InstrumentsDatabase::readInstruments()
{
    Instruments res;

    return res;
}
