#pragma once



#include <QJsonObject>

#include "src/domain/quotation/quotation.h"



struct OperationFifoItem
{
    OperationFifoItem();
    OperationFifoItem(const OperationFifoItem& another) = default;
    ~OperationFifoItem()                                = default;

    OperationFifoItem& operator=(const OperationFifoItem& another) = default;

    void fromJsonObject(const QJsonObject& jsonObject);

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    qint64    quantity;
    Quotation cost;
};



bool operator==(const OperationFifoItem& lhs, const OperationFifoItem& rhs);
