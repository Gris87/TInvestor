#pragma once



#include <QThread>

#include "src/domain/trading/tradinginfo.h"



class IAutoPilotMakeDecisionThread : public QThread
{
    Q_OBJECT

public:
    explicit IAutoPilotMakeDecisionThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IAutoPilotMakeDecisionThread() override = default;

    IAutoPilotMakeDecisionThread(const IAutoPilotMakeDecisionThread& another)            = delete;
    IAutoPilotMakeDecisionThread& operator=(const IAutoPilotMakeDecisionThread& another) = delete;

    virtual void setAccount(const QString& accountId) = 0;
    virtual void setKeepMoney(int value)              = 0;
    virtual void terminateThread()                    = 0;

signals:
    void tradeInstruments(const InstrumentsForTrading& instruments); // Instrument UID => TradingInfo
};
