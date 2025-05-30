#pragma once



#include <QTableWidgetItem>



class MoneyChangeTableItem : public QTableWidgetItem
{
public:
    explicit MoneyChangeTableItem(int type = Type);
    ~MoneyChangeTableItem() override;

    MoneyChangeTableItem(const MoneyChangeTableItem& another)            = delete;
    MoneyChangeTableItem& operator=(const MoneyChangeTableItem& another) = delete;

    void setValue(float value, bool withColor, qint8 precision);

    [[nodiscard]]
    float value() const;

    [[nodiscard]]
    qint8 precision() const;

    bool operator<(const QTableWidgetItem& another) const override;

private:
    float mValue;
    qint8 mPrecision;
};
