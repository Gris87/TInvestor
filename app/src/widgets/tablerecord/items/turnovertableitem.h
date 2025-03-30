#pragma once



#include <QTableWidgetItem>



class TurnoverTableItem : public QTableWidgetItem
{
public:
    explicit TurnoverTableItem(int type = Type);
    ~TurnoverTableItem();

    TurnoverTableItem(const TurnoverTableItem& another)            = delete;
    TurnoverTableItem& operator=(const TurnoverTableItem& another) = delete;

    void   setValue(qint64 value);
    qint64 getValue() const;

    bool operator<(const QTableWidgetItem& another) const;

private:
    qint64 mValue;
};
