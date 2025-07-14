#pragma once



#include <QJsonArray>

#include "src/domain/portfolio/portfoliocategoryitem.h"
#include "src/simdjson/simdjson_wrapped.h"



struct Portfolio
{
    Portfolio();
    Portfolio(const Portfolio& another) = default;
    ~Portfolio()                        = default;

    Portfolio& operator=(const Portfolio& another) = default;

    void fromJsonArray(simdjson::ondemand::array jsonArray); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonArray toJsonArray() const;

    QList<PortfolioCategoryItem> positions;
};



bool operator==(const Portfolio& lhs, const Portfolio& rhs);
