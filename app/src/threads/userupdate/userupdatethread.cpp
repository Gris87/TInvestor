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

    const std::shared_ptr<tinkoff::GetInfoResponse> userInfo = mGrpcClient->getUserInfo(QThread::currentThread());

    if (!QThread::currentThread()->isInterruptionRequested() && userInfo != nullptr)
    {
        User user;

        user.qualified = userInfo->qual_status();
        user.tariff    = QString::fromStdString(userInfo->tariff());

        for (int i = 0; i < userInfo->qualified_for_work_with_size(); ++i)
        {
            user.qualifiedForWorkWith.append(QString::fromStdString(userInfo->qualified_for_work_with(i)));
        }

        const std::shared_ptr<tinkoff::GetAccountsResponse> tinkoffAccounts = mGrpcClient->getAccounts(QThread::currentThread());

        if (!QThread::currentThread()->isInterruptionRequested() && tinkoffAccounts != nullptr)
        {
            Accounts accounts;

            for (int i = 0; i < tinkoffAccounts->accounts_size(); ++i)
            {
                const tinkoff::Account& tinkoffAccount = tinkoffAccounts->accounts(i);

                if (tinkoffAccount.type() == tinkoff::ACCOUNT_TYPE_TINKOFF &&
                    tinkoffAccount.status() == tinkoff::ACCOUNT_STATUS_OPEN &&
                    tinkoffAccount.access_level() == tinkoff::ACCOUNT_ACCESS_LEVEL_FULL_ACCESS)
                {
                    Account account;

                    account.index = i;
                    account.id    = QString::fromStdString(tinkoffAccount.id());
                    account.name  = QString::fromStdString(tinkoffAccount.name());

                    accounts[account.hash()] = account;
                }
            }

            const QMutexLocker lock(mUserStorage->getMutex());

            mUserStorage->setUserInfo(user);
            mUserStorage->setAccounts(accounts);
        }
    }

    qDebug() << "Finish UserUpdateThread";
}
