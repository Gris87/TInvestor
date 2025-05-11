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
    ~IPriceCollectThread() override = default;

    IPriceCollectThread(const IPriceCollectThread& another)            = delete;
    IPriceCollectThread& operator=(const IPriceCollectThread& another) = delete;

    virtual void terminateThread() = 0;

signals:
    void notifyInstrumentsProgress(const QString& message);
    void stocksChanged();
    void pricesChanged();
    void periodicDataChanged();
};
