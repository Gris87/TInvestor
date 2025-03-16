#include "src/widgets/tablerecord/items/paybacktableitem.h"

#include <QDebug>



#define GREEN_COLOR  QColor("#2BD793")
#define RED_COLOR    QColor("#ED6F7E")
#define NORMAL_COLOR QColor("#97AEC4")



PaybackTableItem::PaybackTableItem(int type) :
    QTableWidgetItem(type)
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

    if (mValue > 30 && mValue < 60)
    {
        color = NORMAL_COLOR;
    }
    else
    {
        if (mValue >= 60)
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

float PaybackTableItem::getValue() const
{
    return mValue;
}

bool PaybackTableItem::operator<(const QTableWidgetItem& another) const
{
    float anotherFloat = reinterpret_cast<const PaybackTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
