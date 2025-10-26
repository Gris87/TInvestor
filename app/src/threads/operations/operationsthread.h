#pragma once



#include "src/threads/operations/ioperationsthread.h"

#include <QReadWriteLock>

#include "src/db/operations/ioperationsdatabase.h"
#include "src/domain/quantityandcost/quantityandcost.h"
#include "src/domain/quotation/quotation.h"
#include "src/grpc/igrpcclient.h"
#include "src/grpc/igrpcretryclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/optimizer/ioptimizer.h"
#include "src/utils/timeutils/itimeutils.h"



class OperationsThread : public IOperationsThread
{
    Q_OBJECT

public:
    explicit OperationsThread(
        IOperationsDatabase* operationsDatabase,
        IInstrumentsStorage* instrumentsStorage,
        ILogosStorage*       logosStorage,
        ITimeUtils*          timeUtils,
        IGrpcClient*         grpcClient,
        IGrpcRetryClient*    grpcRetryClient,
        IOptimizer*          optimizer,
        QObject*             parent = nullptr
    );
    ~OperationsThread() override;

    OperationsThread(const OperationsThread& another)            = delete;
    OperationsThread& operator=(const OperationsThread& another) = delete;

    void run() override;

    void setAccountId(const QString& account, const QString& accountId) override;
    void terminateThread() override;

    bool createPortfolioStream();

    bool requestOperations();
    void handleOperationItem(const tinkoff::OperationItem& tinkoffOperation, Operation* res);
    void alignRemainedAndTotalMoneyFromPortfolio(Operation* lastOperation);

#ifdef TESTING_MODE
    void testSetLimitOperations(int limitOperations)
    {
        mLimitOperations = limitOperations;
    }

    void testSetOptimizeSize(int optimizeSize)
    {
        mOptimizeSize = optimizeSize;
    }
#endif

private:
    void cleanRefreshOperations();
    void optimize();

    [[nodiscard]]
    bool isOperationTypeWithExtAccount(tinkoff::OperationType operationType, const QString& positionUid) const;

    QReadWriteLock*                  mRwMutex;
    IOperationsDatabase*             mOperationsDatabase;
    IInstrumentsStorage*             mInstrumentsStorage;
    ILogosStorage*                   mLogosStorage;
    ITimeUtils*                      mTimeUtils;
    IGrpcClient*                     mGrpcClient;
    IGrpcRetryClient*                mGrpcRetryClient;
    IOptimizer*                      mOptimizer;
    QString                          mAccountId;
    std::shared_ptr<PortfolioStream> mPortfolioStream;
    qint64                           mLastCleanRefreshTimestamp;
    qint64                           mLastRequestTimestamp;
    qint64                           mLastOperationTimestamp;
    qint8                            mAmountOfOperationsWithSameTimestamp;
    int                              mAmountOfEntries;
    int                              mLimitOperations;
    int                              mOptimizeSize;
    QString                          mLastPositionUidForExtAccount;
    QSet<QString>                    mOperationsLastDays;
    QuantityAndCostInstruments       mInstruments;
    Quotation                        mInputMoney;
    Quotation                        mMaxInputMoney;
    Quotation                        mTotalYieldWithCommission;
    Quotation                        mRemainedMoney;
    Quotation                        mTotalMoney;
};
