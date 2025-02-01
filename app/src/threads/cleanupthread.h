#pragma once



#include "src/threads/icleanupthread.h"



class CleanupThread : public ICleanupThread
{
    Q_OBJECT

public:
    explicit CleanupThread(QObject *parent = nullptr);
    ~CleanupThread();

    CleanupThread(const CleanupThread &another) = delete;
    CleanupThread& operator=(const CleanupThread &another) = delete;

    void process() override;
};
