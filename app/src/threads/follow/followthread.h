#pragma once



#include "src/threads/follow/ifollowthread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/user/iuserstorage.h"



class FollowThread : public IFollowThread
{
    Q_OBJECT

public:
    explicit FollowThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~FollowThread() override;

    FollowThread(const FollowThread& another)            = delete;
    FollowThread& operator=(const FollowThread& another) = delete;

    void run() override;

    void setAccounts(const QString& account, const QString& anotherAccount) override;
    void terminateThread() override;

    void createPortfolioStream();

private:
    void handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    IUserStorage*                    mUserStorage;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountId;
    QString                          mAnotherAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
};
