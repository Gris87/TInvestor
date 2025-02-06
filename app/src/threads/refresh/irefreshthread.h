#pragma once



#include <QThread>



class IRefreshThread : public QThread
{
public:
    explicit IRefreshThread(QObject *parent = nullptr) : QThread(parent) {}
    virtual ~IRefreshThread() = default;

    IRefreshThread(const IRefreshThread &another) = delete;
    IRefreshThread& operator=(const IRefreshThread &another) = delete;
};
