#pragma once



#include <QThread>



class IPriceCollectThread : public QThread
{
    Q_OBJECT

public:
    explicit IPriceCollectThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    virtual ~IPriceCollectThread() = default;

    IPriceCollectThread(const IPriceCollectThread& another)            = delete;
    IPriceCollectThread& operator=(const IPriceCollectThread& another) = delete;

signals:
    void notifyStocksProgress(const QString& message);
    void stocksChanged();
    void pricesChanged();
    void periodicDataChanged();
};
