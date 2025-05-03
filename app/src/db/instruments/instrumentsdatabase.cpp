#include "src/db/instruments/instrumentsdatabase.h"

#include <QDebug>



InstrumentsDatabase::InstrumentsDatabase() :
    IInstrumentsDatabase()
{
    qDebug() << "Create InstrumentsDatabase";
}

InstrumentsDatabase::~InstrumentsDatabase()
{
    qDebug() << "Destroy InstrumentsDatabase";
}
