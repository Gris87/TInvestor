#pragma once



#include "src/threads/highliquidity/ihighliquiditythread.h"



class HighLiquidityThread : public IHighLiquidityThread
{
    Q_OBJECT

public:
    explicit HighLiquidityThread(QObject* parent = nullptr);
    ~HighLiquidityThread() override;

    HighLiquidityThread(const HighLiquidityThread& another)            = delete;
    HighLiquidityThread& operator=(const HighLiquidityThread& another) = delete;

    void run() override;

    void terminateThread() override;
};
