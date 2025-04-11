#pragma once



#include "src/widgets/orderwaveswidget/iorderwaveswidget.h"

#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QWheelEvent>



class OrderWavesWidget : public IOrderWavesWidget
{
    Q_OBJECT

public:
    explicit OrderWavesWidget(int precision, float priceIncrement, QWidget* parent = nullptr);
    ~OrderWavesWidget();

    OrderWavesWidget(const OrderWavesWidget& another)            = delete;
    OrderWavesWidget& operator=(const OrderWavesWidget& another) = delete;

    void zoom(double factor);

    bool eventFilter(QObject* object, QEvent* event) override;

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
        const QString&           quantityText,
        const QColor&            color
    ) const;

    void setupArrow(float maxBidsPrice, float minAsksPrice, float price);

    [[nodiscard]]
    qint64 normalizePrice(float price) const;
    float  calculateCurrentPrice(float maxBidsPrice, float minAsksPrice);

private:
    QGraphicsScene                  mScene;
    QGraphicsLineItem*              mBottomLine;
    QList<QGraphicsLineItem*>       mAxisLines;
    QList<QGraphicsSimpleTextItem*> mAxisMarkers;
    QList<QGraphicsRectItem*>       mBars;
    QList<QGraphicsSimpleTextItem*> mBarsMarkers;
    QGraphicsItem*                  mArrow;
    int                             mPrecision;
    float                           mPriceIncrement;
    float                           mMinPrice;
    float                           mMaxPrice;
    qint32                          mMaxQuantity;
    bool                            mNeedToFollow;
    int                             mCurrentPricePosX;
    QPointF                         mTargetScenePos;
    QPointF                         mTargetViewportPos;

private slots:
    void sliderMoved(int value);
    void followToCurrentPrice();
};
