#include "src/widgets/tableitems/pricetableitem.h"

#include <QDebug>



constexpr QChar RUBLE = QChar(0x20BD);



PriceTableItem::PriceTableItem(int type) :
    QTableWidgetItem(type),
    mValue(),
    mPrecision()
{
    qDebug() << "Create PriceTableItem";
}

PriceTableItem::~PriceTableItem()
{
    qDebug() << "Destroy PriceTableItem";
}

void PriceTableItem::setValue(float value, qint8 precision)
{
    mValue     = value;
    mPrecision = precision;

    setData(Qt::DisplayRole, QString::number(mValue, 'f', mPrecision) + " " + RUBLE);
}

float PriceTableItem::getValue() const
{
    return mValue;
}

qint8 PriceTableItem::getPrecision() const
{
    return mPrecision;
}

bool PriceTableItem::operator<(const QTableWidgetItem& another) const
{
    const float anotherFloat = dynamic_cast<const PriceTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
