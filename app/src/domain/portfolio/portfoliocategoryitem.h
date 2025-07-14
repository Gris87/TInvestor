#pragma once



#include <QJsonObject>

#include "src/domain/portfolio/portfolioitem.h"
#include "src/simdjson/simdjson_wrapped.h"



struct PortfolioCategoryItem
{
    PortfolioCategoryItem();
    PortfolioCategoryItem(const PortfolioCategoryItem& another) = default;
    ~PortfolioCategoryItem()                                    = default;

    PortfolioCategoryItem& operator=(const PortfolioCategoryItem& another) = default;

    void fromJsonObject(simdjson::ondemand::object jsonObject); // clazy:exclude=function-args-by-ref

    [[nodiscard]]
    QJsonObject toJsonObject() const;

    int                  id;
    QString              name;
    double               cost;
    float                part;
    QList<PortfolioItem> items;
};



bool operator==(const PortfolioCategoryItem& lhs, const PortfolioCategoryItem& rhs);
