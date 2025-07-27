#pragma once



#include "src/threads/autopilotdecisionmaker/iautopilotdecisionmakerthread.h"

#include <QMutex>

#include "src/decisions/idecisionmaker.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"



class AutoPilotDecisionMakerThread : public IAutoPilotDecisionMakerThread
{
    Q_OBJECT

public:
    explicit AutoPilotDecisionMakerThread(
        IStocksStorage* stocksStorage, IDecisionMaker* decisionMaker, IGrpcClient* grpcClient, QObject* parent = nullptr
    );
    ~AutoPilotDecisionMakerThread() override;

    AutoPilotDecisionMakerThread(const AutoPilotDecisionMakerThread& another)            = delete;
    AutoPilotDecisionMakerThread& operator=(const AutoPilotDecisionMakerThread& another) = delete;

    void run() override;

    void setAccountId(const QString& accountId) override;
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
