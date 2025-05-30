#pragma once



#include <QThread>



class IPortfolioLastPriceThread : public QThread
{
    Q_OBJECT

public:
    explicit IPortfolioLastPriceThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IPortfolioLastPriceThread() override = default;

    IPortfolioLastPriceThread(const IPortfolioLastPriceThread& another)            = delete;
    IPortfolioLastPriceThread& operator=(const IPortfolioLastPriceThread& another) = delete;

    virtual void stocksChanged()   = 0;
    virtual void terminateThread() = 0;

signals:
    void lastPriceChanged(const QString& instrumentId, float price);
};
