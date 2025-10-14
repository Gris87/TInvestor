#pragma once



#include <QThread>



class IStockCollectThread : public QThread
{
    Q_OBJECT

public:
    explicit IStockCollectThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IStockCollectThread() override = default;

    IStockCollectThread(const IStockCollectThread& another)            = delete;
    IStockCollectThread& operator=(const IStockCollectThread& another) = delete;

    virtual void terminateThread() = 0;

signals:
    void notifyInstrumentsProgress(const QString& message);
    void stocksChanged();
    void pricesChanged();
    void periodicDataChanged();
};
