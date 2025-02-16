#include "src/storage/user/userstorage.h"

#include <QDebug>



UserStorage::UserStorage(IUserDatabase* userDatabase) :
    IUserStorage(),
    mUser(),
    mUserDatabase(userDatabase)
{
    qDebug() << "Create UserStorage";
}

UserStorage::~UserStorage()
{
    qDebug() << "Destroy UserStorage";
}

void UserStorage::readFromDatabase()
{
    qDebug() << "Reading user data from database";

    mUser = mUserDatabase->readUserInfo();
}

const QString& UserStorage::getToken()
{
    return mUser.token;
}

void UserStorage::setToken(const QString& token)
{
    mUser.token = token;

    mUserDatabase->writeToken(token);
}
