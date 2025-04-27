#include "src/widgets/tableitems/paybacktableitem.h"

#include <QDebug>



constexpr float GREEN_LIMIT  = 60.0f;
constexpr float NORMAL_LIMIT = 30.0f;

const QColor GREEN_COLOR  = QColor("#2BD793"); // clazy:exclude=clazy-non-pod-global-static
const QColor RED_COLOR    = QColor("#ED6F7E"); // clazy:exclude=clazy-non-pod-global-static
const QColor NORMAL_COLOR = QColor("#97AEC4"); // clazy:exclude=clazy-non-pod-global-static



PaybackTableItem::PaybackTableItem(int type) :
    QTableWidgetItem(type),
    mValue()
{
    qDebug() << "Create PaybackTableItem";
}

PaybackTableItem::~PaybackTableItem()
{
    qDebug() << "Destroy PaybackTableItem";
}

void PaybackTableItem::setValue(float value)
{
    mValue = value;

    setData(Qt::DisplayRole, QString::number(mValue, 'f', 2) + "%");

    QColor color;

    if (mValue >= GREEN_LIMIT)
    {
        color = GREEN_COLOR;
    }
    else if (mValue >= NORMAL_LIMIT)
    {
        color = NORMAL_COLOR;
    }
    else
    {
        color = RED_COLOR;
    }

    setForeground(QBrush(color));
}

float PaybackTableItem::getValue() const
{
    return mValue;
}

bool PaybackTableItem::operator<(const QTableWidgetItem& another) const
{
    const float anotherFloat = dynamic_cast<const PaybackTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
