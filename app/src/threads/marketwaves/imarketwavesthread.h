#pragma once



#include <QThread>



class IMarketWavesThread : public QThread
{
public:
    explicit IMarketWavesThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    virtual ~IMarketWavesThread() = default;

    IMarketWavesThread(const IMarketWavesThread& another)            = delete;
    IMarketWavesThread& operator=(const IMarketWavesThread& another) = delete;
};
