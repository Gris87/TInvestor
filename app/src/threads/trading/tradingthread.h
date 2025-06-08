#pragma once



#include "src/threads/trading/itradingthread.h"

#include <QMutex>

#include "src/grpc/igrpcclient.h"



class TradingThread : public ITradingThread
{
    Q_OBJECT

public:
    explicit TradingThread(
        IGrpcClient*   grpcClient,
        const QString& accountId,
        const QString& instrumentId,
        double         expectedCost,
        QObject*       parent = nullptr
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
    QMutex*      mMutex;
    IGrpcClient* mGrpcClient;
    QString      mAccountId;
    QString      mInstrumentId;
    double       mExpectedCost;
};
