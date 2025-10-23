#pragma once



#include "src/domain/stock/stock.h"



class IBollindger
{
public:
    IBollindger()          = default;
    virtual ~IBollindger() = default;

    IBollindger(const IBollindger& another)            = delete;
    IBollindger& operator=(const IBollindger& another) = delete;

    [[nodiscard]]
    virtual float getTopEdge(Stock* stock, int startIndex, int endIndex) const = 0;

    [[nodiscard]]
    virtual float getBottomEdge(Stock* stock, int startIndex, int endIndex) const = 0;

    [[nodiscard]]
    virtual float getTopEdgeOperational(Stock* stock, int startIndex, int endIndex) const = 0;

    [[nodiscard]]
    virtual float getBottomEdgeOperational(Stock* stock, int startIndex, int endIndex) const = 0;
};
