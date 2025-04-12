#pragma once



#include <QTableWidgetItem>



class PaybackTableItem : public QTableWidgetItem
{
public:
    explicit PaybackTableItem(int type = Type);
    ~PaybackTableItem();

    PaybackTableItem(const PaybackTableItem& another)            = delete;
    PaybackTableItem& operator=(const PaybackTableItem& another) = delete;

    void setValue(float value);

    [[nodiscard]]
    float getValue() const;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    float mValue;
};
