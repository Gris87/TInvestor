#include "src/widgets/tableitems/pricechangetableitem.h"

#include <QDebug>



constexpr QChar RUBLE      = QChar(0x20BD);
constexpr float ZERO_LIMIT = 0.0001f;

const QColor GREEN_COLOR  = QColor("#2BD793"); // clazy:exclude=non-pod-global-static
const QColor RED_COLOR    = QColor("#ED6F7E"); // clazy:exclude=non-pod-global-static
const QColor NORMAL_COLOR = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



PriceChangeTableItem::PriceChangeTableItem(int type) :
    QTableWidgetItem(type),
    mValue()
{
    qDebug() << "Create PriceChangeTableItem";
}

PriceChangeTableItem::~PriceChangeTableItem()
{
    qDebug() << "Destroy PriceChangeTableItem";
}

void PriceChangeTableItem::setValue(float value, float fromPrice, int precision)
{
    mValue               = value;
    const QString prefix = mValue > 0 ? "+" : "";

    setData(Qt::DisplayRole, prefix + QString::number(mValue, 'f', 2) + "%");
    setToolTip(fromPrice > 0 ? QObject::tr("From price: %1").arg(fromPrice, 0, 'f', precision) + " " + RUBLE : "");

    QColor color;

    if (mValue > -ZERO_LIMIT && mValue < ZERO_LIMIT)
    {
        color = NORMAL_COLOR;
    }
    else
    {
        if (mValue > 0)
        {
            color = GREEN_COLOR;
        }
        else
        {
            color = RED_COLOR;
        }
    }

    setForeground(QBrush(color));
}

float PriceChangeTableItem::getValue() const
{
    return mValue;
}

bool PriceChangeTableItem::operator<(const QTableWidgetItem& another) const
{
    const float anotherFloat = dynamic_cast<const PriceChangeTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
