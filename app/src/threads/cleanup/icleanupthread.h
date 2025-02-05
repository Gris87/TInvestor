#pragma once



#include <QThread>



class ICleanupThread : public QThread
{
public:
    explicit ICleanupThread(QObject *parent = nullptr) : QThread(parent) {}
    virtual ~ICleanupThread() = default;

    ICleanupThread(const ICleanupThread &another) = delete;
    ICleanupThread& operator=(const ICleanupThread &another) = delete;
};
