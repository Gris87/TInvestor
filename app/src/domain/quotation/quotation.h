#pragma once



#include <QJsonObject>



struct Quotation
{
    Quotation();
    Quotation(const Quotation& another);
    ~Quotation();

    Quotation& operator=(const Quotation& another);

    void        fromJsonObject(const QJsonObject& jsonObject);
    QJsonObject toJsonObject() const;

    qint64 units;
    qint32 nano;
};



bool operator==(const Quotation& lhs, const Quotation& rhs);
