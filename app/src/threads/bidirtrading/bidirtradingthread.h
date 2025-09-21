#pragma once



#include "src/threads/bidirtrading/ibidirtradingthread.h"

#include <QReadWriteLock>

#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/grpc/igrpcretryclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/utils/timeutils/itimeutils.h"
#include "src/utils/tradeutils/itradeutils.h"



class BiDirTradingThread : public IBiDirTradingThread
{
    Q_OBJECT

public:
    explicit BiDirTradingThread(
        IInstrumentsStorage* instrumentsStorage,
        IUserStorage*        userStorage,
        IConfig*             config,
        ITimeUtils*          timeUtils,
        ITradeUtils*         tradeUtils,
        IGrpcClient*         grpcClient,
        IGrpcRetryClient*    grpcRetryClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        const QString&       instrumentId,
        qint64               turnover,
        const QString&       cause,
        QObject*             parent = nullptr
    );
    ~BiDirTradingThread() override;

    BiDirTradingThread(const BiDirTradingThread& another)            = delete;
    BiDirTradingThread& operator=(const BiDirTradingThread& another) = delete;

    void run() override;

    void setTurnover(qint64 turnover) override;

    [[nodiscard]]
    qint64 turnover() const;

    void terminateTrading() override;
    void terminateThread() override;

    bool trade();
    void getInstrumentData();

    void checkIfNeedToCancelAndCreateOrder(
        const QString& orderId, qint64 amountOfLots, const Quotation& price, bool& needToCancel, bool& needToOrder
    );
    void sellWithPrice(const Quotation& price);
    void buyWithPrice(qint64 amountOfLots, const Quotation& price);

    bool isNeedToSellAsap(qint64 timestamp, float part, float yield, float commission);

private:
    void calculateTotalCostAndInstrumentCost(
        const tinkoff::PortfolioResponse& tinkoffPortfolio,
        double&                           totalCost,
        double&                           instrumentCost,
        qint64&                           instrumentLots,
        double&                           instrumentAvgPrice
    );
    IDecisionMakerConfig* chooseDecisionConfig();

    void cancelBuyOrder();
    void cancelSellOrder();

    QReadWriteLock*      mRwMutex;
    IInstrumentsStorage* mInstrumentsStorage;
    IUserStorage*        mUserStorage;
    IConfig*             mConfig;
    ITimeUtils*          mTimeUtils;
    ITradeUtils*         mTradeUtils;
    IGrpcClient*         mGrpcClient;
    IGrpcRetryClient*    mGrpcRetryClient;
    ILogsThread*         mLogsThread;
    QString              mAccountId;
    QString              mInstrumentId;
    qint64               mTurnover;
    bool                 mTerminateTrading;
    qint32               mInstrumentLot;
    Quotation            mMinPriceIncrement;
    QString              mBuyOrderId;
    QString              mSellOrderId;
};
