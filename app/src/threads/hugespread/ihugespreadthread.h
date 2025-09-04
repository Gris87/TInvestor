#pragma once



#include <QThread>

#include "src/domain/trading/tradinginfo.h"



class IHugeSpreadThread : public QThread
{
    Q_OBJECT

public:
    explicit IHugeSpreadThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IHugeSpreadThread() override = default;

    IHugeSpreadThread(const IHugeSpreadThread& another)            = delete;
    IHugeSpreadThread& operator=(const IHugeSpreadThread& another) = delete;

    virtual void setAccountId(const QString& accountId) = 0;
    virtual void setKeepMoney(int value)                = 0;
    virtual void terminateThread()                      = 0;

signals:
    void tradeInstruments(const InstrumentsForTrading& instruments); // Instrument UID => TradingInfo
};
