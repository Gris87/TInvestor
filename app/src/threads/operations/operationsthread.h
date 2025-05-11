#pragma once



#include "src/threads/operations/ioperationsthread.h"

#include "src/domain/operation/operation.h"
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
    void terminateThread() override;

    void      createPortfolioStream();
    Quotation handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);
    void      requestOperations();

private:
    void readOperations();
    void writeOperations();
    void appendOperations(int lastIndex);

    IUserStorage*                    mUserStorage;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountHash;
    QString                          mAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
    qint64                           mLastRequestTimestamp;
    QList<Operation>                 mOperations;
};
