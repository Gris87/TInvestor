#pragma once



#include "src/domain/stock/stock.h"



class IActionDecision
{
public:
    IActionDecision()          = default;
    virtual ~IActionDecision() = default;

    IActionDecision(const IActionDecision& another)            = delete;
    IActionDecision& operator=(const IActionDecision& another) = delete;

    virtual QString makeDecision(Stock* stock, bool dateRange, int dataIndex, float price) = 0;
};
