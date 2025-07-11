#pragma once



#include "src/threads/autopilotmakedecision/iautopilotmakedecisionthread.h"

#include <QMutex>

#include "src/config/iconfig.h"
#include "src/storage/stocks/istocksstorage.h"



class AutoPilotMakeDecisionThread : public IAutoPilotMakeDecisionThread
{
    Q_OBJECT

public:
    explicit AutoPilotMakeDecisionThread(IConfig* config, IStocksStorage* stocksStorage, QObject* parent = nullptr);
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
    QMutex*         mMutex;
    IConfig*        mConfig;
    IStocksStorage* mStocksStorage;
    QString         mAccountId;
    int             mKeepMoney;
};
