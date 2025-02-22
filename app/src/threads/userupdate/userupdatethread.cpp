#include "src/threads/userupdate/userupdatethread.h"

#include <QDebug>
#include <QMutexLocker>



UserUpdateThread::UserUpdateThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent) :
    IUserUpdateThread(parent),
    mUserStorage(userStorage),
    mGrpcClient(grpcClient)
{
    qDebug() << "Create UserUpdateThread";
}

UserUpdateThread::~UserUpdateThread()
{
    qDebug() << "Destroy UserUpdateThread";
}

void UserUpdateThread::run()
{
    qDebug() << "Running UserUpdateThread";

    std::shared_ptr<tinkoff::GetInfoResponse> userInfo = mGrpcClient->getUserInfo();

    if (userInfo != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        User user;

        user.qualified = userInfo->qual_status();
        user.tariff    = QString::fromStdString(userInfo->tariff());

        for (int i = 0; i < userInfo->qualified_for_work_with_size(); ++i)
        {
            user.qualifiedForWorkWith.append(QString::fromStdString(userInfo->qualified_for_work_with(i)));
        }

        std::shared_ptr<tinkoff::GetAccountsResponse> tinkoffAccounts = mGrpcClient->getAccounts();

        if (tinkoffAccounts != nullptr && !QThread::currentThread()->isInterruptionRequested())
        {
            QList<Account> accounts;

            for (int i = 0; i < tinkoffAccounts->accounts_size(); ++i)
            {
                const tinkoff::Account& tinkoffAccount = tinkoffAccounts->accounts(i);

                if (tinkoffAccount.type() == tinkoff::ACCOUNT_TYPE_TINKOFF &&
                    tinkoffAccount.status() == tinkoff::ACCOUNT_STATUS_OPEN &&
                    tinkoffAccount.access_level() == tinkoff::ACCOUNT_ACCESS_LEVEL_FULL_ACCESS)
                {
                    Account account;

                    account.id   = QString::fromStdString(tinkoffAccount.id());
                    account.name = QString::fromStdString(tinkoffAccount.name());

                    accounts.append(account);
                }
            }

            QMutexLocker locker(mUserStorage->getMutex());

            mUserStorage->setUserInfo(user);
            mUserStorage->setAccounts(accounts);
        }
    }

    qDebug() << "Finish UserUpdateThread";
}
