#pragma once



#include <QThread>



class IOrderBookThread : public QThread
{
public:
    explicit IOrderBookThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    virtual ~IOrderBookThread() = default;

    IOrderBookThread(const IOrderBookThread& another)            = delete;
    IOrderBookThread& operator=(const IOrderBookThread& another) = delete;
};
