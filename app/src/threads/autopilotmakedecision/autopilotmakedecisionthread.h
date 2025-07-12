#pragma once



#include "src/threads/autopilotmakedecision/iautopilotmakedecisionthread.h"

#include <QMutex>

#include "src/decisions/idecisionmaker.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"



class AutoPilotMakeDecisionThread : public IAutoPilotMakeDecisionThread
{
    Q_OBJECT

public:
    explicit AutoPilotMakeDecisionThread(
        IStocksStorage* stocksStorage, IDecisionMaker* decisionMaker, IGrpcClient* grpcClient, QObject* parent = nullptr
    );
    ~AutoPilotMakeDecisionThread() override;

    AutoPilotMakeDecisionThread(const AutoPilotMakeDecisionThread& another)            = delete;
    AutoPilotMakeDecisionThread& operator=(const AutoPilotMakeDecisionThread& another) = delete;

    void run() override;

    void setAccount(const QString& accountId) override;
    void setKeepMoney(int value) override;

    [[nodiscard]]
    QString accountId() const;

    [[nodiscard]]
    int keepMoney() const;

    void terminateThread() override;

private:
    Portfolio handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);

    QMutex*         mMutex;
    IStocksStorage* mStocksStorage;
    IDecisionMaker* mDecisionMaker;
    IGrpcClient*    mGrpcClient;
    QString         mAccountId;
    int             mKeepMoney;
};
