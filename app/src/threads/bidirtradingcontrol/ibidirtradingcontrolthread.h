#pragma once



#include <QThread>

#include "src/domain/trading/tradinginfo.h"



class IBiDirTradingControlThread : public QThread
{
    Q_OBJECT

public:
    explicit IBiDirTradingControlThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IBiDirTradingControlThread() override = default;

    IBiDirTradingControlThread(const IBiDirTradingControlThread& another)            = delete;
    IBiDirTradingControlThread& operator=(const IBiDirTradingControlThread& another) = delete;

    virtual void setAccountId(const QString& accountId) = 0;
    virtual void setKeepMoney(int value)                = 0;
    virtual void terminateThread()                      = 0;

signals:
    void tradeInstruments(const InstrumentsForTrading& instruments); // Instrument UID => TradingInfo
};
