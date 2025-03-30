#include "src/widgets/tablerecord/items/turnovertableitem.h"

#include <QDebug>



#define GREEN_COLOR  QColor("#2BD793")
#define RED_COLOR    QColor("#ED6F7E")
#define NORMAL_COLOR QColor("#97AEC4")



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

    if (mValue >= 1000000000)
    {
        text  = QString::number(mValue / 1000000000.0, 'f', 2) + "B " + QChar(0x20BD);
        color = GREEN_COLOR;
    }
    else if (mValue >= 1000000)
    {
        text  = QString::number(mValue / 1000000.0, 'f', 2) + "M " + QChar(0x20BD);
        color = NORMAL_COLOR;
    }
    else
    {
        text  = QString::number(mValue / 1000.0, 'f', 2) + "K " + QChar(0x20BD);
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
