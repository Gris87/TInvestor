#pragma once



#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"



struct Quotation
{
    Quotation();
    Quotation(const Quotation& another) = default;
    ~Quotation()                        = default;

    Quotation& operator=(const Quotation& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64 units;
    qint32 nano;
};



bool operator<(const Quotation& lhs, const Quotation& rhs);
bool operator==(const Quotation& lhs, const Quotation& rhs);
bool operator>(const Quotation& lhs, const Quotation& rhs);
