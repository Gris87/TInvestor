#pragma once



#include "src/domain/stock/stocktableentry.h"



class StocksTableNameLessThan
{
public:
    explicit StocksTableNameLessThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).instrumentTicker < entries->at(r).instrumentTicker;
    }
};

class StocksTablePriceLessThan
{
public:
    explicit StocksTablePriceLessThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).price < entries->at(r).price;
    }
};

class StocksTableDayChangeLessThan
{
public:
    explicit StocksTableDayChangeLessThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).dayChange < entries->at(r).dayChange;
    }
};

class StocksTableDateChangeLessThan
{
public:
    explicit StocksTableDateChangeLessThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).dateChange < entries->at(r).dateChange;
    }
};

class StocksTableTurnoverLessThan
{
public:
    explicit StocksTableTurnoverLessThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).turnover < entries->at(r).turnover;
    }
};

class StocksTablePaybackLessThan
{
public:
    explicit StocksTablePaybackLessThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).payback < entries->at(r).payback;
    }
};

class StocksTableNameGreaterThan
{
public:
    explicit StocksTableNameGreaterThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).instrumentTicker > entries->at(r).instrumentTicker;
    }
};

class StocksTablePriceGreaterThan
{
public:
    explicit StocksTablePriceGreaterThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).price > entries->at(r).price;
    }
};

class StocksTableDayChangeGreaterThan
{
public:
    explicit StocksTableDayChangeGreaterThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).dayChange > entries->at(r).dayChange;
    }
};

class StocksTableDateChangeGreaterThan
{
public:
    explicit StocksTableDateChangeGreaterThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).dateChange > entries->at(r).dateChange;
    }
};

class StocksTableTurnoverGreaterThan
{
public:
    explicit StocksTableTurnoverGreaterThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).turnover > entries->at(r).turnover;
    }
};

class StocksTablePaybackGreaterThan
{
public:
    explicit StocksTablePaybackGreaterThan(QList<StockTableEntry>* _entries) :
        entries(_entries)
    {
    }

    QList<StockTableEntry>* entries;

    bool operator()(int l, int r) const
    {
        return entries->at(l).payback > entries->at(r).payback;
    }
};
