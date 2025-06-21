#pragma once



#include "src/domain/portfolio/portfolioitem.h"



class PortfolioTreeNameLessThan
{
public:
    explicit PortfolioTreeNameLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).instrumentTicker < items->at(r).instrumentTicker;
    }
};

class PortfolioTreeAvailableLessThan
{
public:
    explicit PortfolioTreeAvailableLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).available < items->at(r).available;
    }
};

class PortfolioTreePriceLessThan
{
public:
    explicit PortfolioTreePriceLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).price < items->at(r).price;
    }
};

class PortfolioTreeAvgPriceLessThan
{
public:
    explicit PortfolioTreeAvgPriceLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).avgPriceFifo < items->at(r).avgPriceFifo;
    }
};

class PortfolioTreeCostLessThan
{
public:
    explicit PortfolioTreeCostLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).cost < items->at(r).cost;
    }
};

class PortfolioTreePartLessThan
{
public:
    explicit PortfolioTreePartLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).part < items->at(r).part;
    }
};

class PortfolioTreeYieldLessThan
{
public:
    explicit PortfolioTreeYieldLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).yield < items->at(r).yield;
    }
};

class PortfolioTreeYieldPercentLessThan
{
public:
    explicit PortfolioTreeYieldPercentLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).yieldPercent < items->at(r).yieldPercent;
    }
};

class PortfolioTreeDailyYieldLessThan
{
public:
    explicit PortfolioTreeDailyYieldLessThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).dailyYieldPercent < items->at(r).dailyYieldPercent;
    }
};

class PortfolioTreeNameGreaterThan
{
public:
    explicit PortfolioTreeNameGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).instrumentTicker > items->at(r).instrumentTicker;
    }
};

class PortfolioTreeAvailableGreaterThan
{
public:
    explicit PortfolioTreeAvailableGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).available > items->at(r).available;
    }
};

class PortfolioTreePriceGreaterThan
{
public:
    explicit PortfolioTreePriceGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).price > items->at(r).price;
    }
};

class PortfolioTreeAvgPriceGreaterThan
{
public:
    explicit PortfolioTreeAvgPriceGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).avgPriceFifo > items->at(r).avgPriceFifo;
    }
};

class PortfolioTreeCostGreaterThan
{
public:
    explicit PortfolioTreeCostGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).cost > items->at(r).cost;
    }
};

class PortfolioTreePartGreaterThan
{
public:
    explicit PortfolioTreePartGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).part > items->at(r).part;
    }
};

class PortfolioTreeYieldGreaterThan
{
public:
    explicit PortfolioTreeYieldGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).yield > items->at(r).yield;
    }
};

class PortfolioTreeYieldPercentGreaterThan
{
public:
    explicit PortfolioTreeYieldPercentGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).yieldPercent > items->at(r).yieldPercent;
    }
};

class PortfolioTreeDailyYieldGreaterThan
{
public:
    explicit PortfolioTreeDailyYieldGreaterThan(QList<PortfolioItem>* _items) :
        items(_items)
    {
    }

    QList<PortfolioItem>* items;

    bool operator()(int l, int r) const
    {
        return items->at(l).dailyYieldPercent > items->at(r).dailyYieldPercent;
    }
};
