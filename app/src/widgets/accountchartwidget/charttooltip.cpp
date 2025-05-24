#include "src/widgets/accountchartwidget/charttooltip.h"

#include <QDebug>
#include <QPainter>



ChartTooltip::ChartTooltip(QChart* parent) :
    QGraphicsItem(parent),
    mChart(parent)
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
    path.addRoundedRect(mRect, 5, 5);

    QPointF anchor = mapFromParent(mChart->mapToPosition(mAnchor));

    if (!mRect.contains(anchor) && !mAnchor.isNull())
    {
        QPointF point1;
        QPointF point2;

        // establish the position of the anchor point in relation to mRect
        bool above       = anchor.y() <= mRect.top();
        bool aboveCenter = anchor.y() > mRect.top() && anchor.y() <= mRect.center().y();
        bool belowCenter = anchor.y() > mRect.center().y() && anchor.y() <= mRect.bottom();
        bool below       = anchor.y() > mRect.bottom();

        bool onLeft        = anchor.x() <= mRect.left();
        bool leftOfCenter  = anchor.x() > mRect.left() && anchor.x() <= mRect.center().x();
        bool rightOfCenter = anchor.x() > mRect.center().x() && anchor.x() <= mRect.right();
        bool onRight       = anchor.x() > mRect.right();

        // get the nearest mRect corner.
        qreal x          = (onRight + rightOfCenter) * mRect.width();
        qreal y          = (below + belowCenter) * mRect.height();
        bool  cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
        bool  vertical   = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

        qreal x1 = x + leftOfCenter * 10 - rightOfCenter * 20 + cornerCase * !vertical * (onLeft * 10 - onRight * 20);
        qreal y1 = y + aboveCenter * 10 - belowCenter * 20 + cornerCase * vertical * (above * 10 - below * 20);
        ;
        point1.setX(x1);
        point1.setY(y1);

        qreal x2 = x + leftOfCenter * 20 - rightOfCenter * 10 + cornerCase * !vertical * (onLeft * 20 - onRight * 10);
        ;
        qreal y2 = y + aboveCenter * 20 - belowCenter * 10 + cornerCase * vertical * (above * 20 - below * 10);
        ;
        point2.setX(x2);
        point2.setY(y2);

        path.moveTo(point1);
        path.lineTo(anchor);
        path.lineTo(point2);
        path = path.simplified();
    }

    painter->setBrush(QColor(255, 255, 255));
    painter->drawPath(path);
    painter->drawText(mTextRect, mText);
}

void ChartTooltip::setText(const QString& text)
{
    mText = text;
    QFontMetrics metrics(mFont);
    mTextRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, mText);
    mTextRect.translate(5, 5);
    prepareGeometryChange();
    mRect = mTextRect.adjusted(-5, -5, 5, 5);
}

void ChartTooltip::setAnchor(QPointF point)
{
    mAnchor = point;
}

void ChartTooltip::updateGeometry()
{
    prepareGeometryChange();
    setPos(mChart->mapToPosition(mAnchor) + QPoint(10, -50));
}
