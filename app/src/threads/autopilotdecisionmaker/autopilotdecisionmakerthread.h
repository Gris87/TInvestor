#pragma once



#include "src/threads/autopilotdecisionmaker/iautopilotdecisionmakerthread.h"

#include <QReadWriteLock>

#include "src/decisions/idecisionmaker.h"
#include "src/grpc/igrpcclient.h"
#include "src/storage/stocks/istocksstorage.h"
#include "src/utils/timeutils/itimeutils.h"



class AutoPilotDecisionMakerThread : public IAutoPilotDecisionMakerThread
{
    Q_OBJECT

public:
    explicit AutoPilotDecisionMakerThread(
        IStocksStorage* stocksStorage,
        IConfig*        config,
        IDecisionMaker* decisionMaker,
        ITimeUtils*     timeUtils,
        IGrpcClient*    grpcClient,
        QObject*        parent = nullptr
    );
    ~AutoPilotDecisionMakerThread() override;

    AutoPilotDecisionMakerThread(const AutoPilotDecisionMakerThread& another)            = delete;
    AutoPilotDecisionMakerThread& operator=(const AutoPilotDecisionMakerThread& another) = delete;

    void run() override;

    void setAccountId(const QString& accountId) override;
    void notifyAboutSell(const QString& instrumentId) override;

    void terminateThread() override;

    std::shared_ptr<tinkoff::PortfolioResponse> getValidPortfolio();

private:
    bool            validatePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);
    Portfolio       handlePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);
    InstrumentSells handleGetOperationsByCursorResponse(const tinkoff::GetOperationsByCursorResponse& tinkoffOperations);
    InstrumentSells mergeInstrumentSells(InstrumentSells instrumentsFromOperations);

    QReadWriteLock* mRwMutex;
    IStocksStorage* mStocksStorage;
    IConfig*        mConfig;
    IDecisionMaker* mDecisionMaker;
    ITimeUtils*     mTimeUtils;
    IGrpcClient*    mGrpcClient;
    QString         mAccountId;
    InstrumentSells mSellNotifications;
};
