#include "src/storage/user/userstorage.h"

#include <QDebug>



UserStorage::UserStorage() :
    IUserStorage(),
    mUser()
{
    qDebug() << "Create UserStorage";
}

UserStorage::~UserStorage()
{
    qDebug() << "Destroy UserStorage";
}

void UserStorage::readFromDatabase(IUserDatabase* userDatabase)
{
    qDebug() << "Reading user data from database";

    mUser = userDatabase->readUserInfo();
}

const QString& UserStorage::getToken()
{
    return mUser.token;
}
