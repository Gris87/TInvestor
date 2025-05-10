#pragma once



#include "src/threads/operations/ioperationsthread.h"

#include "src/domain/quotation/quotation.h"
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

    void      createPortfolioStream();
    Quotation handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);
    void requestOperations();

private:
    IUserStorage* mUserStorage;
    IGrpcClient*  mGrpcClient;
    QString       mAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
};
