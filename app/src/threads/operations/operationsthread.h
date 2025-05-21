#pragma once



#include "src/threads/operations/ioperationsthread.h"

#include "src/db/operations/ioperationsdatabase.h"
#include "src/domain/quotation/quotation.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/user/iuserstorage.h"



struct QuantityAndCost
{
    explicit QuantityAndCost() :
        quantity(),
        cost()
    {
    }

    qint64    quantity;
    Quotation cost;
};



class OperationsThread : public IOperationsThread
{
    Q_OBJECT

public:
    explicit OperationsThread(
        IUserStorage* userStorage, IOperationsDatabase* operationsDatabase, IGrpcClient* grpcClient, QObject* parent = nullptr
    );
    ~OperationsThread() override;

    OperationsThread(const OperationsThread& another)            = delete;
    OperationsThread& operator=(const OperationsThread& another) = delete;

    void run() override;

    void setAccount(const QString& account) override;
    void terminateThread() override;

    void createPortfolioStream();

private:
    void      readOperations();
    Quotation handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);
    void      requestOperations();
    Operation handleOperationItem(const tinkoff::OperationItem& tinkoffOperation);

    [[nodiscard]]
    bool isOperationTypeWithExtAccount(tinkoff::OperationType operationType, const QString& positionUid) const;

    IUserStorage*                    mUserStorage;
    IOperationsDatabase*             mOperationsDatabase;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
    qint64                           mLastRequestTimestamp;
    qint64                           mLastOperationTimestamp;
    qint8                            mAmountOfOperationsWithSameTimestamp;
    QString                          mLastPositionUidForExtAccount;
    QMap<QString, QuantityAndCost>   mInstruments; // Instrument Id => QuantityAndCost
    Quotation                        mRemainedMoney;
    Quotation                        mTotalMoney;
};
