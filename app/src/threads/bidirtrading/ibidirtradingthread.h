#pragma once



#include <QThread>

#include "src/domain/trading/bidirtradinginfo.h"



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

    virtual void setMode(BiDirMode bidirMode, const QString& cause) = 0;
    virtual void terminateTrading(bool needToCancelSell)            = 0;
    virtual void terminateThread(bool needToCancelSell)             = 0;

signals:
    void tradingCompleted(const QString& instrumentId);
};
