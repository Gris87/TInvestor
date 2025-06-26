#include "src/storage/user/userstorage.h"

#include <QDebug>



UserStorage::UserStorage(IUserDatabase* userDatabase) :
    IUserStorage(),
    mUserDatabase(userDatabase),
    mRwMutex(new QReadWriteLock()),
    mUser(),
    mAccounts()
{
    qDebug() << "Create UserStorage";
}

UserStorage::~UserStorage()
{
    qDebug() << "Destroy UserStorage";

    delete mRwMutex;
}

void UserStorage::readFromDatabase()
{
    qDebug() << "Reading user data from database";

    mUser     = mUserDatabase->readUserInfo();
    mAccounts = mUserDatabase->readAccounts();
}

void UserStorage::readLock()
{
    mRwMutex->lockForRead();
}

void UserStorage::readUnlock()
{
    mRwMutex->unlock();
}

void UserStorage::writeLock()
{
    mRwMutex->lockForWrite();
}

void UserStorage::writeUnlock()
{
    mRwMutex->unlock();
}

void UserStorage::setToken(const QString& token)
{
    if (mUser.token != token)
    {
        mUser.token = token;

        mUserDatabase->writeToken(token);
    }
}

const QString& UserStorage::getToken()
{
    return mUser.token;
}

void UserStorage::setUserInfo(const User& user)
{
    if (mUser.qualified != user.qualified || mUser.qualifiedForWorkWith != user.qualifiedForWorkWith ||
        mUser.tariff != user.tariff)
    {
        mUser.qualified            = user.qualified;
        mUser.qualifiedForWorkWith = user.qualifiedForWorkWith;
        mUser.setTariff(user.tariff);

        mUserDatabase->writeUserInfo(mUser);
    }
}

bool UserStorage::isQualified()
{
    return mUser.qualified;
}

float UserStorage::getCommission()
{
    return mUser.commission;
}

void UserStorage::setAccounts(const Accounts& accounts)
{
    if (mAccounts != accounts)
    {
        mAccounts = accounts;

        mUserDatabase->writeAccounts(mAccounts);
    }
}

const Accounts& UserStorage::getAccounts()
{
    return mAccounts;
}
