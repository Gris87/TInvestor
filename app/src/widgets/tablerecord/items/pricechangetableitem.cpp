#include "src/widgets/tablerecord/items/pricechangetableitem.h"

#include <QDebug>



PriceChangeTableItem::PriceChangeTableItem(int type) :
    QTableWidgetItem(type)
{
    qDebug() << "Create PriceChangeTableItem";
}

PriceChangeTableItem::~PriceChangeTableItem()
{
    qDebug() << "Destroy PriceChangeTableItem";
}

void PriceChangeTableItem::setValue(float value)
{
    mValue         = value;
    QString prefix = mValue > 0 ? "+" : "";

    setData(Qt::DisplayRole, prefix + QString::number(mValue, 'f', 2) + "%");
}

bool PriceChangeTableItem::operator<(const QTableWidgetItem& another) const
{
    float anotherFloat = reinterpret_cast<const PriceChangeTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
