#pragma once



#include <QThread>

#include "src/domain/trading/tradinginfo.h"



class IHighLiquidityThread : public QThread
{
    Q_OBJECT

public:
    explicit IHighLiquidityThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IHighLiquidityThread() override = default;

    IHighLiquidityThread(const IHighLiquidityThread& another)            = delete;
    IHighLiquidityThread& operator=(const IHighLiquidityThread& another) = delete;

    virtual void setAccountId(const QString& accountId) = 0;
    virtual void terminateThread()                      = 0;

signals:
    void tradeInstruments(const InstrumentsForTrading& instruments); // Instrument UID => TradingInfo
};
