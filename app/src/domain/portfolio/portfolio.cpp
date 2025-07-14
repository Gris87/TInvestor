#include "src/domain/portfolio/portfolio.h"



Portfolio::Portfolio() :
    positions()
{
}

void Portfolio::fromJsonArray(simdjson::ondemand::array jsonArray)
{
    positions.resizeForOverwrite(jsonArray.count_elements());
    int i = 0;

    for (const simdjson::ondemand::object jsonObject : jsonArray)
    {
        positions[i].fromJsonObject(jsonObject);
        ++i;
    }
}

QJsonArray Portfolio::toJsonArray() const
{
    QJsonArray res;

    for (const PortfolioCategoryItem& category : positions)
    {
        res.append(category.toJsonObject());
    }

    return res;
}

bool operator==(const Portfolio& lhs, const Portfolio& rhs)
{
    return lhs.positions == rhs.positions;
}
