#include "src/widgets/tableitems/timetableitem.h"

#include <QDebug>



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";



TimeTableItem::TimeTableItem(int type) :
    QTableWidgetItem(type),
    mValue()
{
    qDebug() << "Create TimeTableItem";
}

TimeTableItem::~TimeTableItem()
{
    qDebug() << "Destroy TimeTableItem";
}

void TimeTableItem::setValue(const QDateTime& value)
{
    mValue = value;

    setData(Qt::DisplayRole, value.toString(DATETIME_FORMAT));
}

QDateTime TimeTableItem::value() const
{
    return mValue;
}

bool TimeTableItem::operator<(const QTableWidgetItem& another) const
{
    const QDateTime anotherFloat = dynamic_cast<const TimeTableItem*>(&another)->mValue;

    return mValue < anotherFloat;
}
