#pragma once



#include <QJsonObject>



struct Quotation
{
    Quotation();
    Quotation(const Quotation& another) = default;
    ~Quotation()                        = default;

    Quotation& operator=(const Quotation& another) = default;

    void fromJsonObject(const QJsonObject& jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64 units;
    qint32 nano;
};



bool operator==(const Quotation& lhs, const Quotation& rhs);
bool operator>(const Quotation& lhs, const Quotation& rhs);
