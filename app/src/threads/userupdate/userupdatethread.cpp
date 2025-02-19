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

    qDebug() << "Finish UserUpdateThread";
}
