#include "src/storage/user/userstorage.h"

#include <QDebug>



UserStorage::UserStorage(IUserDatabase* userDatabase) :
    IUserStorage(),
    mUserDatabase(userDatabase),
    mMutex(new QMutex()),
    mUser(),
    mAccounts()
{
    qDebug() << "Create UserStorage";
}

UserStorage::~UserStorage()
{
    qDebug() << "Destroy UserStorage";

    delete mMutex;
}

void UserStorage::readFromDatabase()
{
    qDebug() << "Reading user data from database";

    mUser     = mUserDatabase->readUserInfo();
    mAccounts = mUserDatabase->readAccounts();
}

QMutex* UserStorage::getMutex()
{
    return mMutex;
}

void UserStorage::lock()
{
    mMutex->lock();
}

void UserStorage::unlock()
{
    mMutex->unlock();
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
