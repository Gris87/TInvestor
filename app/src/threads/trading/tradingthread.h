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
        QObject*             parent = nullptr
    );
    ~TradingThread() override;

    TradingThread(const TradingThread& another)            = delete;
    TradingThread& operator=(const TradingThread& another) = delete;

    void run() override;

    void setExpectedCost(double expectedCost) override;

    [[nodiscard]]
    double expectedCost() const;

    void terminateThread() override;

private:
    [[nodiscard]]
    bool trade();

    [[nodiscard]]
    qint32 getInstrumentLot() const;

    [[nodiscard]]
    double handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    [[nodiscard]]
    bool sell(qint32 lot, double delta, bool sellAll);

    [[nodiscard]]
    bool buy(qint32 lot, double delta);

    QMutex*              mMutex;
    IInstrumentsStorage* mInstrumentsStorage;
    IGrpcClient*         mGrpcClient;
    ILogsThread*         mLogsThread;
    ITimeUtils*          mTimeUtils;
    QString              mAccountId;
    QString              mInstrumentId;
    double               mExpectedCost;
};
