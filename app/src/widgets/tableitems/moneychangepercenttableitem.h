#pragma once



#include <QTableWidgetItem>



class MoneyChangePercentTableItem : public QTableWidgetItem
{
public:
    explicit MoneyChangePercentTableItem(int type = Type);
    ~MoneyChangePercentTableItem() override;

    MoneyChangePercentTableItem(const MoneyChangePercentTableItem& another)            = delete;
    MoneyChangePercentTableItem& operator=(const MoneyChangePercentTableItem& another) = delete;

    void setValue(float value, float fromPrice, qint8 precision);

    [[nodiscard]]
    float value() const;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    float mValue;
};
