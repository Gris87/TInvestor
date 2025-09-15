#pragma once



#include <QThread>

#include "src/domain/trading/bidirtradinginfo.h"



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

    virtual void terminateThread()                      = 0;

signals:
    void tradeInstruments(const InstrumentsForBiDirTrading& instruments); // Instrument UID => BiDirTradingInfo
};
