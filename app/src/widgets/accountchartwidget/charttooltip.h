#pragma once



#include <QGraphicsItem>

#include <QChart>



class ChartTooltip : public QGraphicsItem
{
public:
    explicit ChartTooltip(QChart* parent = nullptr);
    ~ChartTooltip() override;

    ChartTooltip(const ChartTooltip& another)            = delete;
    ChartTooltip& operator=(const ChartTooltip& another) = delete;

    void setText(const QString& text);

    [[nodiscard]]
    QString text() const;

    void setAnchor(QPointF point);
    void updateGeometry();

    [[nodiscard]]
    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QChart* mChart;
    QPointF mAnchor;
    QRectF  mRect;
    QRectF  mTextRect;
    QString mText;
    QFont   mFont;
};
