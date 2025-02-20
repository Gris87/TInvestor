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
    }

    qDebug() << "Finish UserUpdateThread";
}
