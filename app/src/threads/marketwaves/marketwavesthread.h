#pragma once



#include "src/threads/marketwaves/imarketwavesthread.h"



class MarketWavesThread : public IMarketWavesThread
{
    Q_OBJECT

public:
    explicit MarketWavesThread(QObject* parent = nullptr);
    ~MarketWavesThread();

    MarketWavesThread(const MarketWavesThread& another)            = delete;
    MarketWavesThread& operator=(const MarketWavesThread& another) = delete;

    void run() override;
};
