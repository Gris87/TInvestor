#pragma once



#include <QGraphicsView>

#include "src/domain/orderbook/orderbook.h"



class IOrderWavesWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit IOrderWavesWidget(QWidget* parent = nullptr) :
        QGraphicsView(parent)
    {
    }
    virtual ~IOrderWavesWidget() = default;

    IOrderWavesWidget(const IOrderWavesWidget& another)            = delete;
    IOrderWavesWidget& operator=(const IOrderWavesWidget& another) = delete;

    virtual void orderBookChanged(const OrderBook& orderBook) = 0;
    virtual void reset()                                      = 0;
};
