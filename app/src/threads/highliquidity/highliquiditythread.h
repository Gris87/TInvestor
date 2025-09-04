#pragma once



#include "src/threads/highliquidity/ihighliquiditythread.h"

#include <QReadWriteLock>



class HighLiquidityThread : public IHighLiquidityThread
{
    Q_OBJECT

public:
    explicit HighLiquidityThread(QObject* parent = nullptr);
    ~HighLiquidityThread() override;

    HighLiquidityThread(const HighLiquidityThread& another)            = delete;
    HighLiquidityThread& operator=(const HighLiquidityThread& another) = delete;

    void run() override;

    void setAccountId(const QString& accountId) override;
    void setKeepMoney(int value) override;

    [[nodiscard]]
    int keepMoney() const;

    void terminateThread() override;

private:
    QReadWriteLock* mRwMutex;
    QString         mAccountId;
    int             mKeepMoney;
};
