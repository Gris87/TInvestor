#pragma once



#include <QJsonObject>



struct Operation
{
    Operation();
    Operation(const Operation& another) = default;
    ~Operation()                        = default;

    Operation& operator=(const Operation& another) = default;

    void fromJsonObject(const QJsonObject& jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64  timestamp;
    QString instrumentId;
};



bool operator==(const Operation& lhs, const Operation& rhs);
