#pragma once



#include <QJsonObject>



struct Instrument
{
    Instrument();
    Instrument(const Instrument& another) = default;
    ~Instrument()                         = default;

    Instrument& operator=(const Instrument& another) = default;

    void fromJsonObject(const QJsonObject& jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    QString ticker;
    QString name;
    qint8   pricePrecision;
};

using Instruments = QMap<QString, Instrument>; // UID => Instrument



bool operator==(const Instrument& lhs, const Instrument& rhs);
