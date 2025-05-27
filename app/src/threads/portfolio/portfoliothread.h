#pragma once



#include "src/threads/portfolio/iportfoliothread.h"

#include "src/grpc/igrpcclient.h"
#include "src/storage/user/iuserstorage.h"



class PortfolioThread : public IPortfolioThread
{
    Q_OBJECT

public:
    explicit PortfolioThread(IUserStorage* userStorage, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~PortfolioThread() override;

    PortfolioThread(const PortfolioThread& another)            = delete;
    PortfolioThread& operator=(const PortfolioThread& another) = delete;

    void run() override;

    void setAccount(const QString& account) override;
    void terminateThread() override;

    void createPortfolioStream();

private:
    void handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    IUserStorage*                    mUserStorage;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
};
