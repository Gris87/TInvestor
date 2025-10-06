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
    float getTopEdge() const override;

    [[nodiscard]]
    float getBottomEdge() const override;
};
