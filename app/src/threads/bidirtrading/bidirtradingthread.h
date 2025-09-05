#pragma once



#include "src/threads/bidirtrading/ibidirtradingthread.h"



class BiDirTradingThread : public IBiDirTradingThread
{
    Q_OBJECT

public:
    explicit BiDirTradingThread(QObject* parent = nullptr);
    ~BiDirTradingThread() override;

    BiDirTradingThread(const BiDirTradingThread& another)            = delete;
    BiDirTradingThread& operator=(const BiDirTradingThread& another) = delete;

    void run() override;

    void terminateThread() override;
};
