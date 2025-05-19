#include "src/widgets/tableitems/moneychangetableitem.h"

#include <QDebug>



constexpr QChar RUBLE      = QChar(0x20BD);
constexpr float ZERO_LIMIT = 0.0001f;

const QColor GREEN_COLOR  = QColor("#2BD793"); // clazy:exclude=non-pod-global-static
const QColor RED_COLOR    = QColor("#ED6F7E"); // clazy:exclude=non-pod-global-static
const QColor NORMAL_COLOR = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



MoneyChangeTableItem::MoneyChangeTableItem(int type) :
    QTableWidgetItem(type),
    mValue(),
    mPrecision()
{
    qDebug() << "Create MoneyChangeTableItem";
}

MoneyChangeTableItem::~MoneyChangeTableItem()
{
    qDebug() << "Destroy MoneyChangeTableItem";
}

void MoneyChangeTableItem::setValue(float value, bool withPlus, qint8 precision)
{
    mValue     = value;
    mPrecision = precision;

    const QString prefix = mValue > 0 && withPlus ? "+" : "";

    setData(Qt::DisplayRole, prefix + QString::number(mValue, 'f', mPrecision) + " " + RUBLE);

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

float MoneyChangeTableItem::getValue() const
{
    return mValue;
}

qint8 MoneyChangeTableItem::getPrecision() const
{
    return mPrecision;
}

bool MoneyChangeTableItem::operator<(const QTableWidgetItem& another) const
{
    const float anotherFloat = dynamic_cast<const MoneyChangeTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
