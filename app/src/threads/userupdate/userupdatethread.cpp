#include "src/threads/userupdate/userupdatethread.h"

#include <QDateTime>
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

    std::shared_ptr<GetInfoResponse> userInfo = mGrpcClient->getUserInfo();

    if (userInfo != nullptr && !QThread::currentThread()->isInterruptionRequested())
    {
        User user;

        user.qualified = userInfo->qual_status();
        user.tariff    = QString::fromStdString(userInfo->tariff());

        for (int i = 0; i < userInfo->qualified_for_work_with_size(); ++i)
        {
            user.qualifiedForWorkWith.append(QString::fromStdString(userInfo->qualified_for_work_with(i)));
        }

        std::shared_ptr<GetAccountsResponse> accounts = mGrpcClient->getAccounts();

        if (accounts != nullptr && !QThread::currentThread()->isInterruptionRequested())
        {
            qInfo() << accounts->accounts_size();

            for (int i = 0; i < accounts->accounts_size(); ++i)
            {
                const Account& account = accounts->accounts(i);

                qInfo() << account.id();
                qInfo() << account.type();
                qInfo() << QString::fromStdString(account.name());
                qInfo() << account.status();
                qInfo() << account.opened_date().seconds();
                qInfo() << account.closed_date().seconds();
                qInfo() << account.access_level();
            }

            QMutexLocker locker(mUserStorage->getMutex());

            mUserStorage->setUserInfo(user);
        }
    }

    qDebug() << "Finish UserUpdateThread";
}
