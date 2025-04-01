// http://stackoverflow.com/questions/9183050/vertical-qlabel-or-the-equivalent

#include "src/verticallabel/verticallabel.h"

#include <QPainter>

VerticalLabel::VerticalLabel(QWidget* parent) :
    QLabel(parent)
{
}

VerticalLabel::VerticalLabel(const QString& text, QWidget* parent) :
    QLabel(text, parent)
{
}

void VerticalLabel::setColor(const QColor& color)
{
    mColor = color;

    update();
}

const QColor& VerticalLabel::color() const
{
    return mColor;
}

void VerticalLabel::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setPen(mColor);

    painter.translate(0, height());
    painter.rotate(270);

    painter.setFont(font());
    painter.drawText(0, 0, height(), width(), alignment(), text());
}

QSize VerticalLabel::minimumSizeHint() const
{
    QSize s = QLabel::minimumSizeHint();
    return QSize(s.height(), s.width());
}

QSize VerticalLabel::sizeHint() const
{
    QSize s = QLabel::sizeHint();
    return QSize(s.height(), s.width());
}
