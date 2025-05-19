#pragma once



#include <QTableWidgetItem>



class MoneyTableItem : public QTableWidgetItem
{
public:
    explicit MoneyTableItem(int type = Type);
    ~MoneyTableItem() override;

    MoneyTableItem(const MoneyTableItem& another)            = delete;
    MoneyTableItem& operator=(const MoneyTableItem& another) = delete;

    void setValue(float value, qint8 precision);

    [[nodiscard]]
    float getValue() const;

    [[nodiscard]]
    qint8 getPrecision() const;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    float mValue;
    qint8 mPrecision;
};
