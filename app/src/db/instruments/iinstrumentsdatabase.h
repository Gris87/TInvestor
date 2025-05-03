#pragma once



class IInstrumentsDatabase
{
public:
    explicit IInstrumentsDatabase() = default;
    virtual ~IInstrumentsDatabase() = default;

    IInstrumentsDatabase(const IInstrumentsDatabase& another)            = delete;
    IInstrumentsDatabase& operator=(const IInstrumentsDatabase& another) = delete;
};
