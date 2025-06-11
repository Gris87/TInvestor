#pragma once



#include "src/threads/trading/itradingthread.h"

#include <QMutex>

#include "src/grpc/igrpcclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/threads/logs/ilogsthread.h"
#include "src/utils/timeutils/itimeutils.h"



class TradingThread : public ITradingThread
{
    Q_OBJECT

public:
    explicit TradingThread(
        IInstrumentsStorage* instrumentsStorage,
        IGrpcClient*         grpcClient,
        ILogsThread*         logsThread,
        ITimeUtils*          timeUtils,
        const QString&       accountId,
        const QString&       instrumentId,
        double               expectedCost,
        const QString&       cause,
        QObject*             parent = nullptr
    );
    ~TradingThread() override;

    TradingThread(const TradingThread& another)            = delete;
    TradingThread& operator=(const TradingThread& another) = delete;

    void run() override;

    void setExpectedCost(double expectedCost, const QString& cause) override;

    [[nodiscard]]
    double expectedCost() const;

    void terminateThread() override;

private:
    [[nodiscard]]
    bool trade();

    void getInstrumentData();

    [[nodiscard]]
    double handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    [[nodiscard]]
    bool sell(double expected, double delta);

    [[nodiscard]]
    bool sellWithPrice(double expected, double delta, const Quotation& price);

    [[nodiscard]]
    bool sellWithPriceOptimalAmount(double expected, double delta, const Quotation& price);

    [[nodiscard]]
    bool buy(double expected, double delta);

    [[nodiscard]]
    bool buyWithPrice(double expected, double delta, const Quotation& price);

    [[nodiscard]]
    bool buyWithPriceOptimalAmount(double expected, double delta, const Quotation& price);

    void cancelOrder();
    void informAboutOrderState(const tinkoff::OrderState& tinkoffOrder);

    QMutex*              mMutex;
    IInstrumentsStorage* mInstrumentsStorage;
    IGrpcClient*         mGrpcClient;
    ILogsThread*         mLogsThread;
    ITimeUtils*          mTimeUtils;
    QString              mAccountId;
    QString              mInstrumentId;
    double               mExpectedCost;
    qint32               mInstrumentLot;
    qint8                mPricePrecision;
    QString              mOrderId;
    Quotation            mLastOrderPrice;
    double               mLastExpectedCost;
};
