#pragma once



#include "src/threads/hugespread/ihugespreadthread.h"

#include <QReadWriteLock>



class HugeSpreadThread : public IHugeSpreadThread
{
    Q_OBJECT

public:
    explicit HugeSpreadThread(QObject* parent = nullptr);
    ~HugeSpreadThread() override;

    HugeSpreadThread(const HugeSpreadThread& another)            = delete;
    HugeSpreadThread& operator=(const HugeSpreadThread& another) = delete;

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
