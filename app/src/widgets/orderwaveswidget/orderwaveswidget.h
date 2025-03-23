#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidget.h"

#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>



class OrderWavesWidget : public IOrderWavesWidget
{
public:
    explicit OrderWavesWidget(int precision, float priceIncrement, QWidget* parent = nullptr);
    ~OrderWavesWidget();

    OrderWavesWidget(const OrderWavesWidget& another)            = delete;
    OrderWavesWidget& operator=(const OrderWavesWidget& another) = delete;

    void orderBookChanged(const OrderBook& orderBook) override;
    void reset() override;

    QGraphicsRectItem*       getOrCreateBar(int* index);
    QGraphicsSimpleTextItem* getOrCreateBarMarker(int* index);
    void                     deleteBars(int index);
    void                     deleteBarsMarkers(int index);

    void setupBar(
        int                      axisX,
        QGraphicsRectItem*       bar,
        QGraphicsSimpleTextItem* barMarker,
        qint32                   quantity,
        QString                  quantityText,
        const QColor&            color
    );

    qint64 normalizePrice(float price);

private:
    QGraphicsScene                  mScene;
    QGraphicsLineItem*              mBottomLine;
    QList<QGraphicsLineItem*>       mAxisLines;
    QList<QGraphicsSimpleTextItem*> mAxisMarkers;
    QList<QGraphicsRectItem*>       mBars;
    QList<QGraphicsSimpleTextItem*> mBarsMarkers;
    int                             mPrecision;
    float                           mPriceIncrement;
    float                           mMinPrice;
    float                           mMaxPrice;
    qint32                          mMaxQuantity;
};
