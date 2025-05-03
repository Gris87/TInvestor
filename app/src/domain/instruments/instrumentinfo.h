#pragma once



#include <QJsonObject>



struct InstrumentInfo
{
    InstrumentInfo();
    InstrumentInfo(const InstrumentInfo& another) = default;
    ~InstrumentInfo()                             = default;

    InstrumentInfo& operator=(const InstrumentInfo& another) = default;

    void fromJsonObject(const QJsonObject& jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    QString ticker;
    QString name;
};



bool operator==(const InstrumentInfo& lhs, const InstrumentInfo& rhs);
