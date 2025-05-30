#include "src/widgets/tableitems/integertableitem.h"

#include <QDebug>



IntegerTableItem::IntegerTableItem(int type) :
    QTableWidgetItem(type),
    mValue()
{
    qDebug() << "Create IntegerTableItem";
}

IntegerTableItem::~IntegerTableItem()
{
    qDebug() << "Destroy IntegerTableItem";
}

void IntegerTableItem::setValue(qint64 value)
{
    mValue = value;

    setData(Qt::DisplayRole, QString::number(mValue));
}

qint64 IntegerTableItem::value() const
{
    return mValue;
}

bool IntegerTableItem::operator<(const QTableWidgetItem& another) const
{
    const qint64 anotherInt = dynamic_cast<const IntegerTableItem*>(&another)->mValue;

    return mValue < anotherInt;
}
