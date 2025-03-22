#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidget.h"

#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>



class OrderWavesWidget : public IOrderWavesWidget
{
public:
    explicit OrderWavesWidget(float priceIncrement, QWidget* parent = nullptr);
    ~OrderWavesWidget();

    OrderWavesWidget(const OrderWavesWidget& another)            = delete;
    OrderWavesWidget& operator=(const OrderWavesWidget& another) = delete;

    void orderBookChanged(const OrderBook& orderBook) override;
    void reset() override;

private:
    QGraphicsScene                  mScene;
    QGraphicsLineItem*              mBottomLine;
    QList<QGraphicsLineItem*>       mAxisLines;
    QList<QGraphicsSimpleTextItem*> mAxisMarkers;
    QList<QGraphicsRectItem*>       mBids;
    QList<QGraphicsRectItem*>       mSkips;
    QList<QGraphicsRectItem*>       mAsks;
    qint32                          mMaxQuantity;
    float                           mMinPrice;
    float                           mMaxPrice;
    float                           mPriceIncrement;
};
