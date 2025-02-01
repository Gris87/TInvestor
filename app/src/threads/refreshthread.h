#pragma once



#include "src/threads/irefreshthread.h"



class RefreshThread : public IRefreshThread
{
    Q_OBJECT

public:
    explicit RefreshThread(QObject *parent = nullptr);
    ~RefreshThread();

    RefreshThread(const RefreshThread &another) = delete;
    RefreshThread& operator=(const RefreshThread &another) = delete;

    void process() override;
};
