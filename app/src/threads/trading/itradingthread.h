#pragma once



#include <QThread>

#include "src/domain/trading/tradinginfo.h"



class ITradingThread : public QThread
{
    Q_OBJECT

public:
    explicit ITradingThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~ITradingThread() override = default;

    ITradingThread(const ITradingThread& another)            = delete;
    ITradingThread& operator=(const ITradingThread& another) = delete;

    virtual void setAsapMode(AsapMode asapMode)                             = 0;
    virtual void setAvgPrice(float avgPrice)                                = 0;
    virtual void setExpectedCost(double expectedCost, const QString& cause) = 0;
    virtual void terminateThread()                                          = 0;

signals:
    void tradingCompleted(const QString& instrumentId);
};
