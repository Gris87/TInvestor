#pragma once



#include "src/threads/trading/itradingthread.h"

#include <QReadWriteLock>

#include "src/grpc/igrpcclient.h"
#include "src/grpc/igrpcretryclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/utils/timeutils/itimeutils.h"



class TradingThread : public ITradingThread
{
    Q_OBJECT

public:
    explicit TradingThread(
        IInstrumentsStorage* instrumentsStorage,
        IUserStorage*        userStorage,
        ITimeUtils*          timeUtils,
        IGrpcClient*         grpcClient,
        IGrpcRetryClient*    grpcRetryClient,
        ILogsThread*         logsThread,
        const QString&       accountId,
        const QString&       instrumentId,
        AsapMode             asapMode,
        float                avgPrice,
        float                price,
        double               expectedCost,
        const QString&       cause,
        QObject*             parent = nullptr
    );
    ~TradingThread() override;

    TradingThread(const TradingThread& another)            = delete;
    TradingThread& operator=(const TradingThread& another) = delete;

    void run() override;

    void setAsapMode(AsapMode asapMode) override;
    void setAvgPrice(float avgPrice) override;
    void setExpectedCost(double expectedCost, const QString& cause) override;

    [[nodiscard]]
    AsapMode asapMode() const;

    [[nodiscard]]
    float avgPrice() const;

    [[nodiscard]]
    double expectedCost() const;

    void terminateThread() override;

    bool trade();
    void getInstrumentData();
    bool sell(double expected, double delta);
    bool buy(double expected, double delta);

private:
    double handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);
    bool   sellWithPrice(double expected, double delta, const Quotation& price, float marketPrice);
    bool   sellWithPriceOptimalAmount(double expected, double delta, const Quotation& price, float marketPrice);
    bool   buyWithPrice(double expected, double delta, const Quotation& price, float marketPrice);
    bool   buyWithPriceOptimalAmount(double expected, double delta, const Quotation& price, float marketPrice);

    void cancelOrder();
    void informAboutOrderState(const tinkoff::OrderState& tinkoffOrder);

    QReadWriteLock*      mRwMutex;
    IInstrumentsStorage* mInstrumentsStorage;
    IUserStorage*        mUserStorage;
    ITimeUtils*          mTimeUtils;
    IGrpcClient*         mGrpcClient;
    IGrpcRetryClient*    mGrpcRetryClient;
    ILogsThread*         mLogsThread;
    QString              mAccountId;
    QString              mInstrumentId;
    AsapMode             mAsapMode;
    float                mAvgPrice;
    float                mPrice;
    double               mExpectedCost;
    qint32               mInstrumentLot;
    qint8                mPricePrecision;
    Quotation            mMinPriceIncrement;
    QString              mOrderId;
    Quotation            mLastOrderPrice;
    double               mLastExpectedCost;
};
