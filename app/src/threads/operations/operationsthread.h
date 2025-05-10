#pragma once



#include "src/threads/operations/ioperationsthread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/user/iuserstorage.h"



class OperationsThread : public IOperationsThread
{
    Q_OBJECT

public:
    explicit OperationsThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~OperationsThread() override;

    OperationsThread(const OperationsThread& another)            = delete;
    OperationsThread& operator=(const OperationsThread& another) = delete;

    void run() override;

    void setAccount(const QString& account) override;

    void requestOperations();

private:
    IUserStorage* mUserStorage;
    IGrpcClient*  mGrpcClient;
    QString       mAccountId;
};
