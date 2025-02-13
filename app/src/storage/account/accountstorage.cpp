#include "src/storage/account/accountstorage.h"

#include <QDebug>



AccountStorage::AccountStorage() :
    IAccountStorage()
{
    qDebug() << "Create AccountStorage";
}

AccountStorage::~AccountStorage()
{
    qDebug() << "Destroy AccountStorage";
}

void AccountStorage::readFromDatabase(IAccountDatabase* /*accountDatabase*/)
{
    qDebug() << "Reading account data from database";
}
