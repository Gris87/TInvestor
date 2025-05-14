#pragma once



#include <QTableWidgetItem>



class IntegerTableItem : public QTableWidgetItem
{
public:
    explicit IntegerTableItem(int type = Type);
    ~IntegerTableItem() override;

    IntegerTableItem(const IntegerTableItem& another)            = delete;
    IntegerTableItem& operator=(const IntegerTableItem& another) = delete;

    void setValue(qint64 value);

    [[nodiscard]]
    qint64 getValue() const;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    qint64 mValue;
};
