#pragma once



#include <QJsonObject>

#include "src/simdjson/simdjson_wrapped.h"



struct Instrument
{
    Instrument();
    Instrument(const Instrument& another) = default;
    ~Instrument()                         = default;

    Instrument& operator=(const Instrument& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    QString ticker;
    QString name;
    qint32  lot;
    qint8   pricePrecision;
};

using Instruments = QMap<QString, Instrument>; // UID => Instrument



bool operator==(const Instrument& lhs, const Instrument& rhs);
