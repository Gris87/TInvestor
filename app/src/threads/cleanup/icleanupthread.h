#pragma once



#include <QThread>



class ICleanupThread : public QThread
{
    Q_OBJECT

public:
    explicit ICleanupThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~ICleanupThread() override = default;

    ICleanupThread(const ICleanupThread& another)            = delete;
    ICleanupThread& operator=(const ICleanupThread& another) = delete;

    virtual void terminateThread() = 0;
};
