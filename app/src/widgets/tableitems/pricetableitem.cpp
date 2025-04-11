#include "src/widgets/tableitems/pricetableitem.h"

#include <QDebug>



constexpr QChar RUBLE = QChar(0x20BD);



PriceTableItem::PriceTableItem(int type) :
    QTableWidgetItem(type),
    mValue()
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

    setData(Qt::DisplayRole, QString::number(mValue, 'f', precision) + " " + RUBLE);
}

float PriceTableItem::getValue() const
{
    return mValue;
}

bool PriceTableItem::operator<(const QTableWidgetItem& another) const
{
    const float anotherFloat = dynamic_cast<const PriceTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
