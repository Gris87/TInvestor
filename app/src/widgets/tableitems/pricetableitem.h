#pragma once



#include <QTableWidgetItem>



class PriceTableItem : public QTableWidgetItem
{
public:
    explicit PriceTableItem(int type = Type);
    ~PriceTableItem();

    PriceTableItem(const PriceTableItem& another)            = delete;
    PriceTableItem& operator=(const PriceTableItem& another) = delete;

    void setValue(float value, int precision);
    [[nodiscard]]
    float getValue() const;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    float mValue;
};
