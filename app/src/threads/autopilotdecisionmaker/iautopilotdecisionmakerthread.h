#pragma once



#include <QThread>

#include "src/domain/trading/tradinginfo.h"



class IAutoPilotDecisionMakerThread : public QThread
{
    Q_OBJECT

public:
    explicit IAutoPilotDecisionMakerThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IAutoPilotDecisionMakerThread() override = default;

    IAutoPilotDecisionMakerThread(const IAutoPilotDecisionMakerThread& another)            = delete;
    IAutoPilotDecisionMakerThread& operator=(const IAutoPilotDecisionMakerThread& another) = delete;

    virtual void setAccount(const QString& accountId) = 0;
    virtual void setKeepMoney(int value)              = 0;
    virtual void terminateThread()                    = 0;

signals:
    void tradeInstruments(const InstrumentsForTrading& instruments); // Instrument UID => TradingInfo
};
