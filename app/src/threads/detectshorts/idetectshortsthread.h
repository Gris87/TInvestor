#pragma once



#include <QThread>



class IDetectShortsThread : public QThread
{
    Q_OBJECT

public:
    explicit IDetectShortsThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IDetectShortsThread() override = default;

    IDetectShortsThread(const IDetectShortsThread& another)            = delete;
    IDetectShortsThread& operator=(const IDetectShortsThread& another) = delete;

    virtual void terminateThread() = 0;
};
