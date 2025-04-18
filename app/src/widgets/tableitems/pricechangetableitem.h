#pragma once



#include <QTableWidgetItem>



class PriceChangeTableItem : public QTableWidgetItem
{
public:
    explicit PriceChangeTableItem(int type = Type);
    ~PriceChangeTableItem() override;

    PriceChangeTableItem(const PriceChangeTableItem& another)            = delete;
    PriceChangeTableItem& operator=(const PriceChangeTableItem& another) = delete;

    void setValue(float value, float fromPrice, int precision);

    [[nodiscard]]
    float getValue() const;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    float mValue;
};
