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
        qInfo() << userInfo->prem_status();
        qInfo() << userInfo->qual_status();
        qInfo() << userInfo->qualified_for_work_with().size();
        qInfo() << userInfo->tariff();

        std::shared_ptr<GetAccountsResponse> accounts = mGrpcClient->getAccounts();

        if (accounts != nullptr && !QThread::currentThread()->isInterruptionRequested())
        {
            qInfo() << accounts->accounts_size();

            for (int i = 0; i < accounts->accounts_size(); ++i)
            {
                const Account& account = accounts->accounts(i);

                qInfo() << account.id();
                qInfo() << account.type();
                qInfo() << account.name();
                qInfo() << account.status();
                qInfo() << account.opened_date().seconds();
                qInfo() << account.closed_date().seconds();
                qInfo() << account.access_level();
            }
        }
    }

    qDebug() << "Finish UserUpdateThread";
}
