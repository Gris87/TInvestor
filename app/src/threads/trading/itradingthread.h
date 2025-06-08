#pragma once



#include <QThread>



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

    virtual void setExpectedCost(double expectedCost) = 0;
    virtual void terminateThread()                    = 0;

signals:
    void tradingCompleted(const QString& instrumentId);
};
