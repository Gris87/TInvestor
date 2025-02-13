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
