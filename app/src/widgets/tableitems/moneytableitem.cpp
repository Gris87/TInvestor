#include "src/widgets/tableitems/moneytableitem.h"

#include <QDebug>



constexpr QChar RUBLE = QChar(0x20BD);



MoneyTableItem::MoneyTableItem(int type) :
    QTableWidgetItem(type),
    mValue(),
    mPrecision()
{
    qDebug() << "Create MoneyTableItem";
}

MoneyTableItem::~MoneyTableItem()
{
    qDebug() << "Destroy MoneyTableItem";
}

void MoneyTableItem::setValue(float value, qint8 precision)
{
    mValue     = value;
    mPrecision = precision;

    setData(Qt::DisplayRole, QString::number(mValue, 'f', mPrecision) + " " + RUBLE);
}

float MoneyTableItem::getValue() const
{
    return mValue;
}

qint8 MoneyTableItem::getPrecision() const
{
    return mPrecision;
}

bool MoneyTableItem::operator<(const QTableWidgetItem& another) const
{
    const float anotherFloat = dynamic_cast<const MoneyTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
