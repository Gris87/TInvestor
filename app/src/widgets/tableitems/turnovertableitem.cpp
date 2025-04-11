#include "src/widgets/tableitems/turnovertableitem.h"

#include <QDebug>



constexpr QChar  RUBLE        = QChar(0x20BD);
constexpr qint64 GREEN_LIMIT  = 1000000000LL;
constexpr qint64 NORMAL_LIMIT = 1000000LL;
constexpr double BILLIONS     = 1000000000.0;
constexpr qint64 MILLIONS     = 1000000.0;
constexpr qint64 KILOS        = 1000.0;

const QColor GREEN_COLOR  = QColor("#2BD793");
const QColor RED_COLOR    = QColor("#ED6F7E");
const QColor NORMAL_COLOR = QColor("#97AEC4");



TurnoverTableItem::TurnoverTableItem(int type) :
    QTableWidgetItem(type)
{
    qDebug() << "Create TurnoverTableItem";
}

TurnoverTableItem::~TurnoverTableItem()
{
    qDebug() << "Destroy TurnoverTableItem";
}

void TurnoverTableItem::setValue(qint64 value)
{
    mValue = value;

    QString text;
    QColor  color;

    if (mValue >= GREEN_LIMIT)
    {
        text  = QString::number(mValue / BILLIONS, 'f', 2) + "B " + RUBLE;
        color = GREEN_COLOR;
    }
    else if (mValue >= NORMAL_LIMIT)
    {
        text  = QString::number(mValue / MILLIONS, 'f', 2) + "M " + RUBLE;
        color = NORMAL_COLOR;
    }
    else
    {
        text  = QString::number(mValue / KILOS, 'f', 2) + "K " + RUBLE;
        color = RED_COLOR;
    }

    setData(Qt::DisplayRole, text);

    setForeground(QBrush(color));
}

qint64 TurnoverTableItem::getValue() const
{
    return mValue;
}

bool TurnoverTableItem::operator<(const QTableWidgetItem& another) const
{
    qint64 anotherInt = static_cast<const TurnoverTableItem*>(&another)->mValue;

    return mValue < anotherInt;
}
