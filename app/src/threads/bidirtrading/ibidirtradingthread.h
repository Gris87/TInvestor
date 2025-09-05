#pragma once



#include <QThread>



class IBiDirTradingThread : public QThread
{
    Q_OBJECT

public:
    explicit IBiDirTradingThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IBiDirTradingThread() override = default;

    IBiDirTradingThread(const IBiDirTradingThread& another)            = delete;
    IBiDirTradingThread& operator=(const IBiDirTradingThread& another) = delete;

    virtual void terminateThread() = 0;

signals:
    void tradingCompleted(const QString& instrumentId);
};
