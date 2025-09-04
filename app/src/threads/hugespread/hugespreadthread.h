#pragma once



#include "src/threads/hugespread/ihugespreadthread.h"



class HugeSpreadThread : public IHugeSpreadThread
{
    Q_OBJECT

public:
    explicit HugeSpreadThread(QObject* parent = nullptr);
    ~HugeSpreadThread() override;

    HugeSpreadThread(const HugeSpreadThread& another)            = delete;
    HugeSpreadThread& operator=(const HugeSpreadThread& another) = delete;

    void run() override;

    void terminateThread() override;
};
