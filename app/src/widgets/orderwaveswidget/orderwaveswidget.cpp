#include "src/widgets/orderwaveswidget/orderwaveswidget.h"

#include <QDebug>
#include <QScrollBar>
#include <QTimer>



#define LIMIT_BARS         200
#define MAX_BAR_HEIGHT     600
#define BAR_WIDTH          40
#define AXIS_LINE_HEIGHT   10
#define AXIS_MARKER_OFFSET 10
#define BAR_OFFSET         10
#define BAR_MARKER_OFFSET  20
#define ARROW_OFFSET       80
#define ARROW_SIZE         20
#define LINE_COLOR         "#FFFFFF"
#define AXIS_LINE_COLOR    "#FFFFFF"
#define AXIS_MARKER_COLOR  "#FFFFFF"
#define BAR_MARKER_COLOR   "#FFFFFF"
#define GREEN_COLOR        "#2CE89C"
#define RED_COLOR          "#FF7584"
#define GREY_COLOR         "#E5E5E5"
#define ARROW_COLOR        "#FFFFFF"
#define ZOOM_FACTOR_BASE   1.001



OrderWavesWidget::OrderWavesWidget(int precision, float priceIncrement, QWidget* parent) :
    IOrderWavesWidget(parent),
    mScene(),
    mBottomLine(),
    mAxisLines(),
    mAxisMarkers(),
    mBars(),
    mBarsMarkers(),
    mArrow(),
    mPrecision(precision),
    mPriceIncrement(priceIncrement),
    mMinPrice(-1),
    mMaxPrice(-1),
    mMaxQuantity(1),
    mNeedToFollow(true),
    mCurrentPricePosX()
{
    qDebug() << "Create OrderWavesWidget";

    setScene(&mScene);

    setDragMode(DragMode::ScrollHandDrag);

    viewport()->installEventFilter(this);
    setMouseTracking(true);

    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(sliderMoved(int)));
}

OrderWavesWidget::~OrderWavesWidget()
{
    qDebug() << "Destroy OrderWavesWidget";
}

void OrderWavesWidget::zoom(double factor)
{
    mNeedToFollow = false;

    scale(factor, factor);
    centerOn(mTargetScenePos);

    QPointF deltaViewportPos = mTargetViewportPos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
    QPointF viewportCenter   = mapFromScene(mTargetScenePos) - deltaViewportPos;

    centerOn(mapToScene(viewportCenter.toPoint()));
}

bool OrderWavesWidget::eventFilter(QObject* /*object*/, QEvent* event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        mTargetViewportPos = mouseEvent->pos();
        mTargetScenePos    = mapToScene(mouseEvent->pos());
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);

        double angle  = wheelEvent->angleDelta().y();
        double factor = qPow(ZOOM_FACTOR_BASE, angle);

        zoom(factor);

        return true;
    }

    return false;
}

void OrderWavesWidget::orderBookChanged(const OrderBook& orderBook)
{
    QMap<qint64, qint32> priceToQuantityBids; // priceNormal => quantity
    QMap<qint64, qint32> priceToQuantityAsks; // priceNormal => quantity

    float minBidsPrice = -1;
    float maxBidsPrice = -1;
    float minAsksPrice = -1;
    float maxAsksPrice = -1;

    float limitMinPrice = orderBook.price - LIMIT_BARS * mPriceIncrement;
    float limitMaxPrice = orderBook.price + LIMIT_BARS * mPriceIncrement;

    for (int i = 0; i < orderBook.bids.size(); ++i)
    {
        float price = orderBook.bids.at(i).price;

        if (price < limitMinPrice)
        {
            qint64 priceNormal = normalizePrice(limitMinPrice);

            priceToQuantityBids[priceNormal] = 0;

            if (minBidsPrice < 0 || limitMinPrice < minBidsPrice)
            {
                minBidsPrice = limitMinPrice;
            }

            if (maxBidsPrice < 0 || limitMinPrice > maxBidsPrice)
            {
                maxBidsPrice = limitMinPrice;
            }

            break;
        }

        qint32 quantity    = orderBook.bids.at(i).quantity;
        qint64 priceNormal = normalizePrice(price);

        priceToQuantityBids[priceNormal] = quantity;

        if (quantity > mMaxQuantity)
        {
            mMaxQuantity = quantity;
        }

        if (minBidsPrice < 0 || price < minBidsPrice)
        {
            minBidsPrice = price;
        }

        if (maxBidsPrice < 0 || price > maxBidsPrice)
        {
            maxBidsPrice = price;
        }
    }

    for (int i = 0; i < orderBook.asks.size(); ++i)
    {
        float price = orderBook.asks.at(i).price;

        if (price > limitMaxPrice)
        {
            qint64 priceNormal = normalizePrice(limitMaxPrice);

            priceToQuantityAsks[priceNormal] = 0;

            if (minAsksPrice < 0 || limitMaxPrice < minAsksPrice)
            {
                minAsksPrice = limitMaxPrice;
            }

            if (maxAsksPrice < 0 || limitMaxPrice > maxAsksPrice)
            {
                maxAsksPrice = limitMaxPrice;
            }

            break;
        }

        qint32 quantity    = orderBook.asks.at(i).quantity;
        qint64 priceNormal = normalizePrice(price);

        priceToQuantityAsks[priceNormal] = quantity;

        if (quantity > mMaxQuantity)
        {
            mMaxQuantity = quantity;
        }

        if (minAsksPrice < 0 || price < minAsksPrice)
        {
            minAsksPrice = price;
        }

        if (maxAsksPrice < 0 || price > maxAsksPrice)
        {
            maxAsksPrice = price;
        }
    }

    if (mMinPrice < 0 || minBidsPrice < mMinPrice)
    {
        mMinPrice = minBidsPrice;
    }

    if (mMinPrice < 0 || minAsksPrice < mMinPrice)
    {
        mMinPrice = minAsksPrice;
    }

    if (mMaxPrice < 0 || maxBidsPrice > mMaxPrice)
    {
        mMaxPrice = maxBidsPrice;
    }

    if (mMaxPrice < 0 || maxAsksPrice > mMaxPrice)
    {
        mMaxPrice = maxAsksPrice;
    }

    if (mMinPrice < 0 && mMaxPrice < 0)
    {
        return;
    }

    qint64 minBidsPriceNormal = normalizePrice(minBidsPrice);
    qint64 maxBidsPriceNormal = normalizePrice(maxBidsPrice);
    qint64 minAsksPriceNormal = normalizePrice(minAsksPrice);
    qint64 maxAsksPriceNormal = normalizePrice(maxAsksPrice);

    int amountOfBars = qRound((mMaxPrice - mMinPrice) / mPriceIncrement) + 1;

    if (mBottomLine != nullptr)
    {
        mBottomLine->setLine(0, 0, amountOfBars * BAR_WIDTH, 0);
    }
    else
    {
        mBottomLine = mScene.addLine(0, 0, amountOfBars * BAR_WIDTH, 0, QPen(QColor(LINE_COLOR)));
    }

    int barIndex       = 0;
    int barMarkerIndex = 0;

    for (int i = 0; i < amountOfBars; ++i)
    {
        QGraphicsSimpleTextItem* axisMarker;

        int     axisX          = i * BAR_WIDTH + BAR_WIDTH / 2;
        float   price          = amountOfBars > 1 ? mMinPrice + (mMaxPrice - mMinPrice) * i / (amountOfBars - 1) : mMinPrice;
        qint64  priceNormal    = normalizePrice(price);
        QString axisMarkerText = QString::number(price, 'f', mPrecision);

        if (i < mAxisLines.size())
        {
            axisMarker = mAxisMarkers.at(i);

            mAxisLines.at(i)->setLine(axisX, -AXIS_LINE_HEIGHT / 2, axisX, AXIS_LINE_HEIGHT / 2);
            axisMarker->setText(axisMarkerText + " " + QChar(0x20BD));
        }
        else
        {
            axisMarker = mScene.addSimpleText(axisMarkerText + " " + QChar(0x20BD));
            axisMarker->setBrush(QBrush(QColor(AXIS_MARKER_COLOR)));
            axisMarker->setRotation(45);

            mAxisLines.append(
                mScene.addLine(axisX, -AXIS_LINE_HEIGHT / 2, axisX, AXIS_LINE_HEIGHT / 2, QPen(QColor(AXIS_LINE_COLOR)))
            );
            mAxisMarkers.append(axisMarker);
        }

        axisMarker->setPos(axisX, AXIS_MARKER_OFFSET);

        if (priceNormal < minBidsPriceNormal)
        {
            if (priceToQuantityBids.size() >= 50)
            {
                setupBar(axisX, getOrCreateBar(&barIndex), getOrCreateBarMarker(&barMarkerIndex), 0, "?", QColor(GREY_COLOR));
            }
        }

        if (priceNormal >= minBidsPriceNormal && priceNormal <= maxBidsPriceNormal)
        {
            qint32 quantity = priceToQuantityBids[priceNormal];

            setupBar(
                axisX,
                getOrCreateBar(&barIndex),
                getOrCreateBarMarker(&barMarkerIndex),
                quantity,
                QString::number(quantity),
                QColor(GREEN_COLOR)
            );
        }

        if (priceNormal > maxBidsPriceNormal && priceNormal < minAsksPriceNormal)
        {
            setupBar(axisX, getOrCreateBar(&barIndex), nullptr, 0, "", QColor(GREY_COLOR));
        }

        if (priceNormal >= minAsksPriceNormal && priceNormal <= maxAsksPriceNormal)
        {
            qint32 quantity = priceToQuantityAsks[priceNormal];

            setupBar(
                axisX,
                getOrCreateBar(&barIndex),
                getOrCreateBarMarker(&barMarkerIndex),
                quantity,
                QString::number(quantity),
                QColor(RED_COLOR)
            );
        }

        if (priceNormal > maxAsksPriceNormal)
        {
            if (priceToQuantityAsks.size() >= 50)
            {
                setupBar(axisX, getOrCreateBar(&barIndex), getOrCreateBarMarker(&barMarkerIndex), 0, "?", QColor(GREY_COLOR));
            }
        }
    }

    deleteBars(barIndex);
    deleteBarsMarkers(barMarkerIndex);

    setupArrow(maxBidsPrice, minAsksPrice, orderBook.price);

    viewport()->update();

    mCurrentPricePosX = mMaxPrice > mMinPrice
                            ? ((calculateCurrentPrice(maxBidsPrice, minAsksPrice) - mMinPrice) / (mMaxPrice - mMinPrice)) *
                                  amountOfBars * BAR_WIDTH
                            : 0;

    if (mNeedToFollow)
    {
        QTimer::singleShot(0, this, SLOT(followToCurrentPrice()));
    }
}

void OrderWavesWidget::reset()
{
    resetTransform();
    followToCurrentPrice();
}

QGraphicsRectItem* OrderWavesWidget::getOrCreateBar(int* index)
{
    QGraphicsRectItem* res;

    if (*index < mBars.size())
    {
        res = mBars.at(*index);
    }
    else
    {
        res = mScene.addRect(0, 0, 0, 0);
        mBars.append(res);
    }

    *index += 1;

    return res;
}

QGraphicsSimpleTextItem* OrderWavesWidget::getOrCreateBarMarker(int* index)
{
    QGraphicsSimpleTextItem* res;

    if (*index < mBarsMarkers.size())
    {
        res = mBarsMarkers.at(*index);
    }
    else
    {
        res = mScene.addSimpleText("");
        res->setBrush(QBrush(QColor(BAR_MARKER_COLOR)));
        mBarsMarkers.append(res);
    }

    *index += 1;

    return res;
}

void OrderWavesWidget::deleteBars(int index)
{
    for (int i = index; i < mBars.size(); ++i)
    {
        QGraphicsRectItem* bar = mBars.at(i);

        mScene.removeItem(bar);
        delete bar;
    }

    mBars.resize(index);
}

void OrderWavesWidget::deleteBarsMarkers(int index)
{
    for (int i = index; i < mBarsMarkers.size(); ++i)
    {
        QGraphicsSimpleTextItem* marker = mBarsMarkers.at(i);

        mScene.removeItem(marker);
        delete marker;
    }

    mBarsMarkers.resize(index);
}

void OrderWavesWidget::setupBar(
    int                      axisX,
    QGraphicsRectItem*       bar,
    QGraphicsSimpleTextItem* barMarker,
    qint32                   quantity,
    QString                  quantityText,
    const QColor&            color
)
{
    int barHeight = MAX_BAR_HEIGHT * quantity / mMaxQuantity;

    if (barHeight < 4)
    {
        barHeight = 4;
    }

    QColor brushColor = color;
    brushColor.setAlphaF(0.5f);

    bar->setRect(axisX - BAR_WIDTH / 2, -BAR_OFFSET - barHeight, BAR_WIDTH, barHeight);
    bar->setPen(QPen(color));
    bar->setBrush(QBrush(brushColor));

    if (barMarker != nullptr)
    {
        barMarker->setText(quantityText);
        barMarker->setPos(axisX - barMarker->boundingRect().width() / 2, -BAR_OFFSET - barHeight - BAR_MARKER_OFFSET);
    }
}

void OrderWavesWidget::setupArrow(float maxBidsPrice, float minAsksPrice, float price)
{
    if (mArrow != nullptr)
    {
        mScene.removeItem(mArrow);

        delete mArrow;
        mArrow = nullptr;
    }

    if (maxBidsPrice < 0 && minAsksPrice < 0)
    {
        return;
    }

    float startPrice;
    float endPrice = price;

    if (maxBidsPrice >= 0 && minAsksPrice >= 0)
    {
        float middlePrice = (maxBidsPrice + minAsksPrice) / 2;

        if (price < middlePrice)
        {
            startPrice = minAsksPrice;
        }
        else if (price > middlePrice)
        {
            startPrice = maxBidsPrice;
        }
        else
        {
            startPrice = middlePrice;
            endPrice   = middlePrice;
        }
    }
    else
    {
        startPrice = price;
    }

    int amountOfBars   = qRound((mMaxPrice - mMinPrice) / mPriceIncrement) + 1;
    int startPricePosX = ((startPrice - mMinPrice) / (mMaxPrice - mMinPrice)) * amountOfBars * BAR_WIDTH;
    int endPricePosX   = ((endPrice - mMinPrice) / (mMaxPrice - mMinPrice)) * amountOfBars * BAR_WIDTH;

    if (startPrice == endPrice)
    {
        mArrow = mScene.addEllipse(
            endPricePosX - ARROW_SIZE / 2,
            -ARROW_OFFSET - ARROW_SIZE / 2,
            ARROW_SIZE,
            ARROW_SIZE,
            QPen(QBrush(QColor(ARROW_COLOR)), 2)
        );
    }
    else
    {
        QPolygonF arrowPolygon;

        int sign = endPrice > startPrice ? 1 : -1;

        arrowPolygon.append(QPointF(startPricePosX, -ARROW_OFFSET + ARROW_SIZE / 10));
        arrowPolygon.append(QPointF(endPricePosX, -ARROW_OFFSET + ARROW_SIZE / 10));
        arrowPolygon.append(QPointF(endPricePosX, -ARROW_OFFSET + ARROW_SIZE / 4));
        arrowPolygon.append(QPointF(endPricePosX + sign * ARROW_SIZE / 4, -ARROW_OFFSET));
        arrowPolygon.append(QPointF(endPricePosX, -ARROW_OFFSET - ARROW_SIZE / 4));
        arrowPolygon.append(QPointF(endPricePosX, -ARROW_OFFSET - ARROW_SIZE / 10));
        arrowPolygon.append(QPointF(startPricePosX, -ARROW_OFFSET - ARROW_SIZE / 10));

        mArrow = mScene.addPolygon(arrowPolygon, QPen(QColor(ARROW_COLOR)), QBrush(QColor(ARROW_COLOR)));
    }
}

qint64 OrderWavesWidget::normalizePrice(float price)
{
    return qRound64(price * qPow(10, mPrecision));
}

float OrderWavesWidget::calculateCurrentPrice(float maxBidsPrice, float minAsksPrice)
{
    if (maxBidsPrice >= 0)
    {
        if (minAsksPrice >= 0)
        {
            return (maxBidsPrice + minAsksPrice) / 2;
        }
        else
        {
            return maxBidsPrice;
        }
    }
    else
    {
        if (minAsksPrice >= 0)
        {
            return minAsksPrice;
        }
    }

    return 0;
}

void OrderWavesWidget::sliderMoved(int /*value*/)
{
    mNeedToFollow = false;
}

void OrderWavesWidget::followToCurrentPrice()
{
    centerOn(mCurrentPricePosX, 0);
    mNeedToFollow = true;
}
