#pragma once



#include <QThread>



class ILastPriceThread : public QThread
{
public:
    explicit ILastPriceThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    virtual ~ILastPriceThread() = default;

    ILastPriceThread(const ILastPriceThread& another)            = delete;
    ILastPriceThread& operator=(const ILastPriceThread& another) = delete;
};
