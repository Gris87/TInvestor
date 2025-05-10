#pragma once



#include <QThread>



class ILastPriceThread : public QThread
{
    Q_OBJECT

public:
    explicit ILastPriceThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~ILastPriceThread() override = default;

    ILastPriceThread(const ILastPriceThread& another)            = delete;
    ILastPriceThread& operator=(const ILastPriceThread& another) = delete;

    virtual void stocksChanged()   = 0;
    virtual void terminateThread() = 0;

signals:
    void lastPriceChanged(const QString& instrumentId);
};
