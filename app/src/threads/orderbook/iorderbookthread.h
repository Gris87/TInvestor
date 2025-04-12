#pragma once



#include <QThread>

#include "src/domain/orderbook/orderbook.h"
#include "src/domain/stocks/stock.h"



class IOrderBookThread : public QThread
{
    Q_OBJECT

public:
    explicit IOrderBookThread(QObject* parent = nullptr) :
        QThread(parent)
    {
    }
    ~IOrderBookThread() override = default;

    IOrderBookThread(const IOrderBookThread& another)            = delete;
    IOrderBookThread& operator=(const IOrderBookThread& another) = delete;

    virtual void setStock(Stock* stock) = 0;
    virtual void terminateThread()      = 0;

signals:
    void orderBookChanged(const OrderBook& orderBook);
};
