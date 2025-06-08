#pragma once



#include "src/threads/trading/itradingthreadfactory.h"



class TradingThreadFactory : public ITradingThreadFactory
{
public:
    TradingThreadFactory();
    ~TradingThreadFactory() override;

    TradingThreadFactory(const TradingThreadFactory& another)            = delete;
    TradingThreadFactory& operator=(const TradingThreadFactory& another) = delete;

    ITradingThread* newInstance(IGrpcClient* grpcClient, QObject* parent) const override;
};
