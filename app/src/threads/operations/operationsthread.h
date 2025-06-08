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
        fifoItems(),
        costFifo(),
        costWavg()
    {
    }

    qint64                   quantity;
    QList<OperationFifoItem> fifoItems;
    Quotation                costFifo;
    Quotation                costWavg;
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

    void createPositionsStream();

private:
    void      readOperations();
    Quotation handlePositionsResponse(const tinkoff::PositionsResponse& tinkoffPositions);
    Quotation handlePositionsResponse(const tinkoff::PositionData& tinkoffPositions);
    void      requestOperations();
    Operation handleOperationItem(const tinkoff::OperationItem& tinkoffOperation);

    [[nodiscard]]
    bool isOperationTypeWithExtAccount(tinkoff::OperationType operationType, const QString& positionUid) const;

    IUserStorage*                    mUserStorage;
    IOperationsDatabase*             mOperationsDatabase;
    IGrpcClient*                     mGrpcClient;
    QString                          mAccountId;
    std::shared_ptr<PositionsStream> mPositionsStream;
    qint64                           mLastRequestTimestamp;
    qint64                           mLastOperationTimestamp;
    qint8                            mAmountOfOperationsWithSameTimestamp;
    QString                          mLastPositionUidForExtAccount;
    QMap<QString, QuantityAndCost>   mInstruments; // Instrument Id => QuantityAndCost
    Quotation                        mInputMoney;
    Quotation                        mMaxInputMoney;
    Quotation                        mTotalYieldWithCommission;
    Quotation                        mRemainedMoney;
    Quotation                        mTotalMoney;
};
