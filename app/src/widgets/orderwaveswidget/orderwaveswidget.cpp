#include "src/widgets/orderwaveswidget/orderwaveswidget.h"

#include <QDebug>
#include <QScrollBar>
#include <QTimer>
#include <algorithm>



constexpr QChar  RUBLE              = QChar(0x20BD);
constexpr int    LIMIT_BARS         = 200;
constexpr int    MAX_ORDERS_COUNT   = 50;
constexpr int    MAX_BAR_HEIGHT     = 600;
constexpr int    BAR_WIDTH          = 40;
constexpr float  BAR_OPACITY        = 0.5f;
constexpr int    AXIS_LINE_HEIGHT   = 10;
constexpr int    AXIS_MARKER_OFFSET = 10;
constexpr int    AXIS_MARKER_ANGLE  = 45;
constexpr int    BAR_OFFSET         = 10;
constexpr int    BAR_MARKER_OFFSET  = 20;
constexpr int    ARROW_OFFSET       = 80;
constexpr int    ARROW_SIZE         = 20;
constexpr double ZOOM_FACTOR_BASE   = 1.001;

const QColor LINE_COLOR        = QColor("#FFFFFF");
const QColor AXIS_LINE_COLOR   = QColor("#FFFFFF");
const QColor AXIS_MARKER_COLOR = QColor("#FFFFFF");
const QColor BAR_MARKER_COLOR  = QColor("#FFFFFF");
const QColor GREEN_COLOR       = QColor("#2CE89C");
const QColor RED_COLOR         = QColor("#FF7584");
const QColor GREY_COLOR        = QColor("#E5E5E5");
const QColor ARROW_COLOR       = QColor("#FFFFFF");



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

    // NOLINTNEXTLINE(readability-magic-numbers)
    const QPointF deltaViewportPos = mTargetViewportPos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
    const QPointF viewportCenter   = mapFromScene(mTargetScenePos) - deltaViewportPos;

    centerOn(mapToScene(viewportCenter.toPoint()));
}

bool OrderWavesWidget::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

        mTargetViewportPos = mouseEvent->pos();
        mTargetScenePos    = mapToScene(mouseEvent->pos());
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = dynamic_cast<QWheelEvent*>(event);

        const double angle  = wheelEvent->angleDelta().y();
        const double factor = qPow(ZOOM_FACTOR_BASE, angle);

        zoom(factor);

        return true;
    }

    return IOrderWavesWidget::eventFilter(object, event);
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void OrderWavesWidget::orderBookChanged(const OrderBook& orderBook)
{
    QMap<qint64, qint32> priceToQuantityBids; // priceNormal => quantity
    QMap<qint64, qint32> priceToQuantityAsks; // priceNormal => quantity

    float minBidsPrice = -1;
    float maxBidsPrice = -1;
    float minAsksPrice = -1;
    float maxAsksPrice = -1;

    const float limitMinPrice = orderBook.price - (LIMIT_BARS * mPriceIncrement);
    const float limitMaxPrice = orderBook.price + (LIMIT_BARS * mPriceIncrement);

    for (const OrderBookData& bid : orderBook.bids)
    {
        const float price = bid.price;

        if (price < limitMinPrice)
        {
            const qint64 priceNormal = normalizePrice(limitMinPrice);

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

        const qint32 quantity    = bid.quantity;
        const qint64 priceNormal = normalizePrice(price);

        priceToQuantityBids[priceNormal] = quantity;

        mMaxQuantity = qMax(quantity, mMaxQuantity);

        if (minBidsPrice < 0 || price < minBidsPrice)
        {
            minBidsPrice = price;
        }

        if (maxBidsPrice < 0 || price > maxBidsPrice)
        {
            maxBidsPrice = price;
        }
    }

    for (const OrderBookData& ask : orderBook.asks)
    {
        const float price = ask.price;

        if (price > limitMaxPrice)
        {
            const qint64 priceNormal = normalizePrice(limitMaxPrice);

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

        const qint32 quantity    = ask.quantity;
        const qint64 priceNormal = normalizePrice(price);

        priceToQuantityAsks[priceNormal] = quantity;

        mMaxQuantity = qMax(quantity, mMaxQuantity);

        if (minAsksPrice < 0 || price < minAsksPrice)
        {
            minAsksPrice = price;
        }

        if (maxAsksPrice < 0 || price > maxAsksPrice)
        {
            maxAsksPrice = price;
        }
    }

    if (mMinPrice < 0 || (minBidsPrice > 0 && minBidsPrice < mMinPrice))
    {
        mMinPrice = minBidsPrice;
    }

    if (mMinPrice < 0 || (minAsksPrice > 0 && minAsksPrice < mMinPrice))
    {
        mMinPrice = minAsksPrice;
    }

    if (mMaxPrice < 0 || (maxBidsPrice > 0 && maxBidsPrice > mMaxPrice))
    {
        mMaxPrice = maxBidsPrice;
    }

    if (mMaxPrice < 0 || (maxAsksPrice > 0 && maxAsksPrice > mMaxPrice))
    {
        mMaxPrice = maxAsksPrice;
    }

    if (mMinPrice < 0 && mMaxPrice < 0)
    {
        return;
    }

    const qint64 minBidsPriceNormal = normalizePrice(minBidsPrice);
    const qint64 maxBidsPriceNormal = normalizePrice(maxBidsPrice);
    const qint64 minAsksPriceNormal = normalizePrice(minAsksPrice);
    const qint64 maxAsksPriceNormal = normalizePrice(maxAsksPrice);

    const int amountOfBars = qRound((mMaxPrice - mMinPrice) / mPriceIncrement) + 1;

    if (mBottomLine != nullptr)
    {
        mBottomLine->setLine(0, 0, amountOfBars * BAR_WIDTH, 0);
    }
    else
    {
        mBottomLine = mScene.addLine(0, 0, amountOfBars * BAR_WIDTH, 0, QPen(LINE_COLOR));
    }

    int barIndex       = 0;
    int barMarkerIndex = 0;

    for (int i = 0; i < amountOfBars; ++i)
    {
        QGraphicsLineItem*       axisLine   = nullptr;
        QGraphicsSimpleTextItem* axisMarker = nullptr;

        const int     axisX       = (i * BAR_WIDTH) + (BAR_WIDTH / 2);
        const float   price       = amountOfBars > 1 ? mMinPrice + ((mMaxPrice - mMinPrice) * i / (amountOfBars - 1)) : mMinPrice;
        const qint64  priceNormal = normalizePrice(price);
        const QString axisMarkerText = QString::number(price, 'f', mPrecision);

        if (i < mAxisLines.size())
        {
            axisLine   = mAxisLines.at(i);
            axisMarker = mAxisMarkers.at(i);

            // NOLINTNEXTLINE(readability-magic-numbers)
            axisLine->setLine(axisX, -AXIS_LINE_HEIGHT / 2.0f, axisX, AXIS_LINE_HEIGHT / 2.0f);
            axisMarker->setText(axisMarkerText + " " + RUBLE);
        }
        else
        {
            // NOLINTNEXTLINE(readability-magic-numbers)
            axisLine = mScene.addLine(axisX, -AXIS_LINE_HEIGHT / 2.0f, axisX, AXIS_LINE_HEIGHT / 2.0f, QPen(AXIS_LINE_COLOR));

            axisMarker = mScene.addSimpleText(axisMarkerText + " " + RUBLE);
            axisMarker->setBrush(QBrush(AXIS_MARKER_COLOR));
            axisMarker->setRotation(AXIS_MARKER_ANGLE);

            mAxisLines.append(axisLine);
            mAxisMarkers.append(axisMarker);
        }

        axisMarker->setPos(axisX, AXIS_MARKER_OFFSET);

        if (priceNormal < minBidsPriceNormal)
        {
            if (priceToQuantityBids.size() >= MAX_ORDERS_COUNT)
            {
                setupBar(axisX, getOrCreateBar(&barIndex), getOrCreateBarMarker(&barMarkerIndex), 0, "?", GREY_COLOR);
            }
        }

        if (priceNormal >= minBidsPriceNormal && priceNormal <= maxBidsPriceNormal)
        {
            const qint32 quantity = priceToQuantityBids[priceNormal];

            setupBar(
                axisX,
                getOrCreateBar(&barIndex),
                getOrCreateBarMarker(&barMarkerIndex),
                quantity,
                QString::number(quantity),
                GREEN_COLOR
            );
        }

        if (priceNormal > maxBidsPriceNormal && priceNormal < minAsksPriceNormal)
        {
            setupBar(axisX, getOrCreateBar(&barIndex), nullptr, 0, "", GREY_COLOR);
        }

        if (priceNormal >= minAsksPriceNormal && priceNormal <= maxAsksPriceNormal)
        {
            const qint32 quantity = priceToQuantityAsks[priceNormal];

            setupBar(
                axisX,
                getOrCreateBar(&barIndex),
                getOrCreateBarMarker(&barMarkerIndex),
                quantity,
                QString::number(quantity),
                RED_COLOR
            );
        }

        if (priceNormal > maxAsksPriceNormal)
        {
            if (priceToQuantityAsks.size() >= MAX_ORDERS_COUNT)
            {
                setupBar(axisX, getOrCreateBar(&barIndex), getOrCreateBarMarker(&barMarkerIndex), 0, "?", GREY_COLOR);
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
// NOLINTEND(readability-function-cognitive-complexity)

void OrderWavesWidget::reset()
{
    resetTransform();
    followToCurrentPrice();
}

QGraphicsRectItem* OrderWavesWidget::getOrCreateBar(int* index)
{
    QGraphicsRectItem* res = nullptr;

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
    QGraphicsSimpleTextItem* res = nullptr;

    if (*index < mBarsMarkers.size())
    {
        res = mBarsMarkers.at(*index);
    }
    else
    {
        res = mScene.addSimpleText("");
        res->setBrush(QBrush(BAR_MARKER_COLOR));
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
    const QString&           quantityText,
    const QColor&            color
) const
{
    int barHeight = MAX_BAR_HEIGHT * quantity / mMaxQuantity;

    barHeight = qMax(barHeight, 4);

    QColor brushColor = color;
    brushColor.setAlphaF(BAR_OPACITY);

    bar->setRect(axisX - (BAR_WIDTH / 2.0f), -BAR_OFFSET - barHeight, BAR_WIDTH, barHeight); // NOLINT(readability-magic-numbers)
    bar->setPen(QPen(color));
    bar->setBrush(QBrush(brushColor));

    if (barMarker != nullptr)
    {
        barMarker->setText(quantityText);
        // NOLINTNEXTLINE(readability-magic-numbers)
        barMarker->setPos(axisX - (barMarker->boundingRect().width() / 2.0f), -BAR_OFFSET - barHeight - BAR_MARKER_OFFSET);
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

    float startPrice = 0.0f;
    float endPrice   = price;

    if (maxBidsPrice >= 0 && minAsksPrice >= 0)
    {
        const float middlePrice = (maxBidsPrice + minAsksPrice) / 2;

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

    const int amountOfBars   = qRound((mMaxPrice - mMinPrice) / mPriceIncrement) + 1;
    const int startPricePosX = ((startPrice - mMinPrice) / (mMaxPrice - mMinPrice)) * amountOfBars * BAR_WIDTH;
    const int endPricePosX   = ((endPrice - mMinPrice) / (mMaxPrice - mMinPrice)) * amountOfBars * BAR_WIDTH;

    if (startPrice == endPrice)
    {
        mArrow = mScene.addEllipse(
            endPricePosX - (ARROW_SIZE / 2.0f),  // NOLINT(readability-magic-numbers)
            -ARROW_OFFSET - (ARROW_SIZE / 2.0f), // NOLINT(readability-magic-numbers)
            ARROW_SIZE,
            ARROW_SIZE,
            QPen(QBrush(ARROW_COLOR), 2)
        );
    }
    else
    {
        QPolygonF arrowPolygon;

        const int sign = endPrice > startPrice ? 1 : -1;

        // NOLINTBEGIN(readability-magic-numbers)
        arrowPolygon.append(QPointF(startPricePosX, -ARROW_OFFSET + (ARROW_SIZE / 10.0f)));
        arrowPolygon.append(QPointF(endPricePosX, -ARROW_OFFSET + (ARROW_SIZE / 10.0f)));
        arrowPolygon.append(QPointF(endPricePosX, -ARROW_OFFSET + (ARROW_SIZE / 4.0f)));
        arrowPolygon.append(QPointF(endPricePosX + (sign * ARROW_SIZE / 4.0f), -ARROW_OFFSET));
        arrowPolygon.append(QPointF(endPricePosX, -ARROW_OFFSET - (ARROW_SIZE / 4.0f)));
        arrowPolygon.append(QPointF(endPricePosX, -ARROW_OFFSET - (ARROW_SIZE / 10.0f)));
        arrowPolygon.append(QPointF(startPricePosX, -ARROW_OFFSET - (ARROW_SIZE / 10.0f)));
        // NOLINTEND(readability-magic-numbers)

        mArrow = mScene.addPolygon(arrowPolygon, QPen(ARROW_COLOR), QBrush(ARROW_COLOR));
    }
}

qint64 OrderWavesWidget::normalizePrice(float price) const
{
    return qRound64(price * qPow(10, mPrecision)); // NOLINT(readability-magic-numbers)
}

float OrderWavesWidget::calculateCurrentPrice(float maxBidsPrice, float minAsksPrice)
{
    float res = 0.0;

    if (maxBidsPrice >= 0)
    {
        if (minAsksPrice >= 0)
        {
            res = (maxBidsPrice + minAsksPrice) / 2;
        }
        else
        {
            res = maxBidsPrice;
        }
    }
    else
    {
        if (minAsksPrice >= 0)
        {
            res = minAsksPrice;
        }
    }

    return res;
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
