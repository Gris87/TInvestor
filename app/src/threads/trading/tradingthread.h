#pragma once



#include "src/threads/trading/itradingthread.h"

#include <QMutex>

#include "src/grpc/igrpcclient.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/threads/logs/ilogsthread.h"



class TradingThread : public ITradingThread
{
    Q_OBJECT

public:
    explicit TradingThread(
        IInstrumentsStorage* instrumentsStorage,
        IGrpcClient*         grpcClient,
        ILogsThread*         logsThread,
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
    bool   trade();
    double handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);
    bool   sell(qint32 lot, double delta, bool sellAll);
    bool   buy(qint32 lot, double delta);

    QMutex*              mMutex;
    IInstrumentsStorage* mInstrumentsStorage;
    IGrpcClient*         mGrpcClient;
    ILogsThread*         mLogsThread;
    QString              mAccountId;
    QString              mInstrumentId;
    double               mExpectedCost;
};
