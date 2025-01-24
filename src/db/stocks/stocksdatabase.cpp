#include "stocksdatabase.h"

#include <QDebug>



StocksDatabase::StocksDatabase(QObject *parent) :
    QObject{parent}
{
    qDebug() << "Create StocksDatabase";
}

StocksDatabase::~StocksDatabase()
{
    qDebug() << "Destroy StocksDatabase";
}
