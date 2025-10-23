#pragma once



#include "src/utils/bollindger/ibollindger.h"



class Bollindger : public IBollindger
{
public:
    explicit Bollindger();
    ~Bollindger() override;

    Bollindger(const Bollindger& another)            = delete;
    Bollindger& operator=(const Bollindger& another) = delete;

    [[nodiscard]]
    float getTopEdge(Stock* stock, int startIndex, int endIndex) const override;

    [[nodiscard]]
    float getBottomEdge(Stock* stock, int startIndex, int endIndex) const override;

    [[nodiscard]]
    double getMovingAverage(Stock* stock, int startIndex, int endIndex) const;

    [[nodiscard]]
    double getStandartDeviation(Stock* stock, int startIndex, int endIndex, double mean) const;

    [[nodiscard]]
    float getTopEdgeOperational(Stock* stock, int startIndex, int endIndex) const override;

    [[nodiscard]]
    float getBottomEdgeOperational(Stock* stock, int startIndex, int endIndex) const override;

    [[nodiscard]]
    double getMovingAverageOperational(Stock* stock, int startIndex, int endIndex) const;

    [[nodiscard]]
    double getStandartDeviationOperational(Stock* stock, int startIndex, int endIndex, double mean) const;
};
