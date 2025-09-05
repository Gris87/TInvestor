#pragma once



#include "src/threads/bidirtradingcontrol/ibidirtradingcontrolthread.h"

#include <QReadWriteLock>



class BiDirTradingControlThread : public IBiDirTradingControlThread
{
    Q_OBJECT

public:
    explicit BiDirTradingControlThread(QObject* parent = nullptr);
    ~BiDirTradingControlThread() override;

    BiDirTradingControlThread(const BiDirTradingControlThread& another)            = delete;
    BiDirTradingControlThread& operator=(const BiDirTradingControlThread& another) = delete;

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
