#include "src/widgets/tablerecord/items/pricechangetableitem.h"

#include <QDebug>



#define GREEN_COLOR  QColor("#2BD793")
#define RED_COLOR    QColor("#ED6F7E")
#define NORMAL_COLOR QColor("#97AEC4")



PriceChangeTableItem::PriceChangeTableItem(int type) :
    QTableWidgetItem(type)
{
    qDebug() << "Create PriceChangeTableItem";
}

PriceChangeTableItem::~PriceChangeTableItem()
{
    qDebug() << "Destroy PriceChangeTableItem";
}

void PriceChangeTableItem::setValue(float value, float fromPrice, int precision)
{
    mValue         = value;
    QString prefix = mValue > 0 ? "+" : "";

    setData(Qt::DisplayRole, prefix + QString::number(mValue, 'f', 2) + "%");
    setToolTip(fromPrice > 0 ? QObject::tr("From price: %1").arg(fromPrice, 0, 'f', precision) + " " + QChar(0x20BD) : "");

    QColor color;

    if (mValue > -0.0001f && mValue < 0.0001f)
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
    float anotherFloat = static_cast<const PriceChangeTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
