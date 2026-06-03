#include "src/widgets/accountchartwidget/charttooltip.h"

#include <QDebug>
#include <QPainter>
#include <QPen>



constexpr int TEXT_BOX_WIDTH         = 150;
constexpr int TEXT_BOX_HEIGHT        = 150;
constexpr int MARGIN                 = 5;
constexpr int BUBBLE_CONNECT_WIDTH_1 = 30;
constexpr int BUBBLE_CONNECT_WIDTH_2 = 40;
constexpr int BUBBLE_POS_X           = -30;
constexpr int BUBBLE_POS_Y           = 20;
const QColor  BUBBLE_COLOR           = QColor("#FFFFFF"); // clazy:exclude=non-pod-global-static
const QColor  TEXT_COLOR             = QColor("#000000"); // clazy:exclude=non-pod-global-static



ChartTooltip::ChartTooltip(QChart* parent) :
    QGraphicsItem(parent),
    mChart(parent),
    mAnchor(),
    mRect(),
    mTextRect(),
    mText(),
    mFont()
{
    qDebug() << "Create ChartTooltip";
}

ChartTooltip::~ChartTooltip()
{
    qDebug() << "Destroy ChartTooltip";
}

QRectF ChartTooltip::boundingRect() const
{
    const QPointF anchor = mapFromParent(mChart->mapToPosition(mAnchor));

    QRectF rect;

    rect.setLeft(qMin(mRect.left(), anchor.x()));
    rect.setRight(qMax(mRect.right(), anchor.x()));
    rect.setTop(qMin(mRect.top(), anchor.y()));
    rect.setBottom(qMax(mRect.bottom(), anchor.y()));

    return rect;
}

void ChartTooltip::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    QPainterPath path;
    path.addRoundedRect(mRect, MARGIN, MARGIN);

    const QPointF anchor = mapFromParent(mChart->mapToPosition(mAnchor));

    QPointF point1;
    QPointF point2;

    // establish the position of the anchor point in relation to mRect
    const bool above       = anchor.y() <= mRect.top();
    const bool aboveCenter = anchor.y() > mRect.top() && anchor.y() <= mRect.center().y();
    const bool belowCenter = anchor.y() > mRect.center().y() && anchor.y() <= mRect.bottom();
    const bool below       = anchor.y() > mRect.bottom();

    const bool onLeft        = anchor.x() <= mRect.left();
    const bool leftOfCenter  = anchor.x() > mRect.left() && anchor.x() <= mRect.center().x();
    const bool rightOfCenter = anchor.x() > mRect.center().x() && anchor.x() <= mRect.right();
    const bool onRight       = anchor.x() > mRect.right();

    // get the nearest mRect corner.
    const qreal x          = (static_cast<int>(onRight) + static_cast<int>(rightOfCenter)) * mRect.width();
    const qreal y          = (static_cast<int>(below) + static_cast<int>(belowCenter)) * mRect.height();
    const bool  cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
    const bool  vertical   = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

    const qreal x1 = x + (static_cast<int>(leftOfCenter) * BUBBLE_CONNECT_WIDTH_1) -
                     (static_cast<int>(rightOfCenter) * BUBBLE_CONNECT_WIDTH_2) +
                     (static_cast<int>(cornerCase) * static_cast<int>(!vertical) *
                      (static_cast<int>(onLeft) * BUBBLE_CONNECT_WIDTH_1 - static_cast<int>(onRight) * BUBBLE_CONNECT_WIDTH_2));
    const qreal y1 = y + (static_cast<int>(aboveCenter) * BUBBLE_CONNECT_WIDTH_1) -
                     (static_cast<int>(belowCenter) * BUBBLE_CONNECT_WIDTH_2) +
                     (static_cast<int>(cornerCase) * static_cast<int>(vertical) *
                      (static_cast<int>(above) * BUBBLE_CONNECT_WIDTH_1 - static_cast<int>(below) * BUBBLE_CONNECT_WIDTH_2));
    const qreal x2 = x + (static_cast<int>(leftOfCenter) * BUBBLE_CONNECT_WIDTH_2) -
                     (static_cast<int>(rightOfCenter) * BUBBLE_CONNECT_WIDTH_1) +
                     (static_cast<int>(cornerCase) * static_cast<int>(!vertical) *
                      (static_cast<int>(onLeft) * BUBBLE_CONNECT_WIDTH_2 - static_cast<int>(onRight) * BUBBLE_CONNECT_WIDTH_1));
    const qreal y2 = y + (static_cast<int>(aboveCenter) * BUBBLE_CONNECT_WIDTH_2) -
                     (static_cast<int>(belowCenter) * BUBBLE_CONNECT_WIDTH_1) +
                     (static_cast<int>(cornerCase) * static_cast<int>(vertical) *
                      (static_cast<int>(above) * BUBBLE_CONNECT_WIDTH_2 - static_cast<int>(below) * BUBBLE_CONNECT_WIDTH_1));

    point1.setX(x1);
    point1.setY(y1);
    point2.setX(x2);
    point2.setY(y2);

    path.moveTo(point1);
    path.lineTo(anchor);
    path.lineTo(point2);
    path = path.simplified();

    painter->setBrush(BUBBLE_COLOR);
    painter->setPen(QPen(TEXT_COLOR));
    painter->drawPath(path);
    painter->drawText(mTextRect, mText);
}

void ChartTooltip::setText(const QString& text)
{
    const QFontMetrics metrics(mFont);

    mText = text;

    mTextRect = metrics.boundingRect(QRect(0, 0, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT), Qt::AlignLeft, mText);
    mTextRect.translate(MARGIN, MARGIN);
    mRect = mTextRect.adjusted(-MARGIN, -MARGIN, MARGIN, MARGIN);

    prepareGeometryChange();
}

QString ChartTooltip::text() const
{
    return mText;
}

void ChartTooltip::setAnchor(QPointF point)
{
    mAnchor = point;
}

void ChartTooltip::updateGeometry()
{
    prepareGeometryChange();
    setPos(mChart->mapToPosition(mAnchor) + QPoint(BUBBLE_POS_X, BUBBLE_POS_Y));
}
