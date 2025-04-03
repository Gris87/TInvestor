#pragma once



#include <QTableWidgetItem>



class PriceChangeTableItem : public QTableWidgetItem
{
public:
    explicit PriceChangeTableItem(int type = Type);
    ~PriceChangeTableItem();

    PriceChangeTableItem(const PriceChangeTableItem& another)            = delete;
    PriceChangeTableItem& operator=(const PriceChangeTableItem& another) = delete;

    void  setValue(float value, float fromPrice, int precision);
    float getValue() const;

    bool operator<(const QTableWidgetItem& another) const;

private:
    float mValue;
};
