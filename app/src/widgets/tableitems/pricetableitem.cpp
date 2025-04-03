#include "src/widgets/tableitems/pricetableitem.h"

#include <QDebug>



PriceTableItem::PriceTableItem(int type) :
    QTableWidgetItem(type)
{
    qDebug() << "Create PriceTableItem";
}

PriceTableItem::~PriceTableItem()
{
    qDebug() << "Destroy PriceTableItem";
}

void PriceTableItem::setValue(float value, int precision)
{
    mValue = value;

    setData(Qt::DisplayRole, QString::number(mValue, 'f', precision) + " " + QChar(0x20BD));
}

float PriceTableItem::getValue() const
{
    return mValue;
}

bool PriceTableItem::operator<(const QTableWidgetItem& another) const
{
    float anotherFloat = static_cast<const PriceTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
