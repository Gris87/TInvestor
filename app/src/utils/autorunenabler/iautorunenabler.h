#pragma once



class IAutorunEnabler
{
public:
    IAutorunEnabler()          = default;
    virtual ~IAutorunEnabler() = default;

    IAutorunEnabler(const IAutorunEnabler& another)            = delete;
    IAutorunEnabler& operator=(const IAutorunEnabler& another) = delete;

    virtual void setEnabled(bool enabled) = 0;
};
