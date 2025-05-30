#pragma once



#include <QTableWidgetItem>

#include <QDateTime>



class TimeTableItem : public QTableWidgetItem
{
public:
    explicit TimeTableItem(int type = Type);
    ~TimeTableItem() override;

    TimeTableItem(const TimeTableItem& another)            = delete;
    TimeTableItem& operator=(const TimeTableItem& another) = delete;

    void setValue(const QDateTime& value);

    [[nodiscard]]
    QDateTime value() const;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    QDateTime mValue;
};
