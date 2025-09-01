#pragma once



#include "src/threads/trading/itradingthread.h"

#include <QReadWriteLock>

#include "src/grpc/igrpcclient.h"
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
        ILogsThread*         logsThread,
        const QString&       accountId,
        const QString&       instrumentId,
        bool                 asap,
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

    void setAsap(bool asap) override;
    void setAvgPrice(float avgPrice) override;
    void setExpectedCost(double expectedCost, const QString& cause) override;

    [[nodiscard]]
    bool asap() const;

    [[nodiscard]]
    float avgPrice() const;

    [[nodiscard]]
    double expectedCost() const;

    void terminateThread() override;

    [[nodiscard]]
    bool trade();

    void getInstrumentData();

    [[nodiscard]]
    bool sell(double expected, double delta);

    [[nodiscard]]
    bool buy(double expected, double delta);

private:
    [[nodiscard]]
    double handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    [[nodiscard]]
    bool sellWithPrice(double expected, double delta, const Quotation& price, float marketPrice);

    [[nodiscard]]
    bool sellWithPriceOptimalAmount(double expected, double delta, const Quotation& price, float marketPrice);

    [[nodiscard]]
    bool buyWithPrice(double expected, double delta, const Quotation& price, float marketPrice);

    [[nodiscard]]
    bool buyWithPriceOptimalAmount(double expected, double delta, const Quotation& price, float marketPrice);

    void cancelOrder();
    void informAboutOrderState(const tinkoff::OrderState& tinkoffOrder);

    QReadWriteLock*      mRwMutex;
    IInstrumentsStorage* mInstrumentsStorage;
    IUserStorage*        mUserStorage;
    ITimeUtils*          mTimeUtils;
    IGrpcClient*         mGrpcClient;
    ILogsThread*         mLogsThread;
    QString              mAccountId;
    QString              mInstrumentId;
    bool                 mAsap;
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
