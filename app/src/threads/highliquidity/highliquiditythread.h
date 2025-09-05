#pragma once



#include "src/threads/highliquidity/ihighliquiditythread.h"

#include <QTimeZone>

#include "src/config/iconfig.h"
#include "src/grpc/igrpcclient.h"
#include "src/utils/timeutils/itimeutils.h"



class HighLiquidityThread : public IHighLiquidityThread
{
    Q_OBJECT

public:
    explicit HighLiquidityThread(IConfig* config, ITimeUtils* timeUtils, IGrpcClient* grpcClient, QObject* parent = nullptr);
    ~HighLiquidityThread() override;

    HighLiquidityThread(const HighLiquidityThread& another)            = delete;
    HighLiquidityThread& operator=(const HighLiquidityThread& another) = delete;

    void run() override;

    void makeDecisionBaseOnTimestamp(qint64 timestamp);
    void buyEtf();
    void sellEtf();

    void setAccountId(const QString& accountId) override;

    void terminateThread() override;

private:
    bool validatePortfolioResponse(const tinkoff::PortfolioResponse& tinkoffPortfolio);
    void calculateMoneyAndTotalCost(
        const tinkoff::PortfolioResponse& tinkoffPortfolio, double& money, double& totalCost, bool& etfFound
    );

    IConfig*     mConfig;
    ITimeUtils*  mTimeUtils;
    IGrpcClient* mGrpcClient;
    QTimeZone    mMoscowTimezone;
    QString      mAccountId;
};
