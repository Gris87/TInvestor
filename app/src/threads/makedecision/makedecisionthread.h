#pragma once



#include "src/threads/makedecision/imakedecisionthread.h"

#include <QMutex>

#include "src/config/iconfig.h"
#include "src/storage/stocks/istocksstorage.h"



class MakeDecisionThread : public IMakeDecisionThread
{
    Q_OBJECT

public:
    explicit MakeDecisionThread(IConfig* config, IStocksStorage* stocksStorage, QObject* parent = nullptr);
    ~MakeDecisionThread() override;

    MakeDecisionThread(const MakeDecisionThread& another)            = delete;
    MakeDecisionThread& operator=(const MakeDecisionThread& another) = delete;

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
