#pragma once



#include "src/threads/detectdividends/idetectdividendsthread.h"

#include "src/storage/stocks/istocksstorage.h"



class DetectDividendsThread : public IDetectDividendsThread
{
    Q_OBJECT

public:
    explicit DetectDividendsThread(IStocksStorage* stocksStorage, QObject* parent = nullptr);
    ~DetectDividendsThread() override;

    DetectDividendsThread(const DetectDividendsThread& another)            = delete;
    DetectDividendsThread& operator=(const DetectDividendsThread& another) = delete;

    void run() override;

    void terminateThread() override;

private:
    IStocksStorage* mStocksStorage;
};
