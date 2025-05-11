#pragma once



#include "src/threads/userupdate/iuserupdatethread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/user/iuserstorage.h"



class UserUpdateThread : public IUserUpdateThread
{
    Q_OBJECT

public:
    explicit UserUpdateThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~UserUpdateThread() override;

    UserUpdateThread(const UserUpdateThread& another)            = delete;
    UserUpdateThread& operator=(const UserUpdateThread& another) = delete;

    void run() override;

    void terminateThread() override;

private:
    IUserStorage* mUserStorage;
    IGrpcClient*  mGrpcClient;
};
