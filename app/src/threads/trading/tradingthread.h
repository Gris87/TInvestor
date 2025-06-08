#pragma once



#include "src/threads/trading/itradingthread.h"

#include "src/grpc/igrpcclient.h"



class TradingThread : public ITradingThread
{
    Q_OBJECT

public:
    explicit TradingThread(IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~TradingThread() override;

    TradingThread(const TradingThread& another)            = delete;
    TradingThread& operator=(const TradingThread& another) = delete;

    void run() override;

    void setAccountId(const QString& accountId) override;
    void terminateThread() override;

private:
    IGrpcClient* mGrpcClient;
    QString      mAccountId;
};
