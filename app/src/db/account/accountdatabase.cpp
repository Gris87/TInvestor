#include "src/db/account/accountdatabase.h"

#include <QDebug>



AccountDatabase::AccountDatabase() :
    IAccountDatabase()
{
    qDebug() << "Create AccountDatabase";
}

AccountDatabase::~AccountDatabase()
{
    qDebug() << "Destroy AccountDatabase";
}
