#pragma once



class IBollindger
{
public:
    IBollindger()          = default;
    virtual ~IBollindger() = default;

    IBollindger(const IBollindger& another)            = delete;
    IBollindger& operator=(const IBollindger& another) = delete;

    [[nodiscard]]
    virtual float getTopEdge() const = 0;

    [[nodiscard]]
    virtual float getBottomEdge() const = 0;
};
