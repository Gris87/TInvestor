#include "src/widgets/tablerecord/items/paybacktableitem.h"

#include <QDebug>



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
}

bool PaybackTableItem::operator<(const QTableWidgetItem& another) const
{
    float anotherFloat = reinterpret_cast<const PaybackTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
