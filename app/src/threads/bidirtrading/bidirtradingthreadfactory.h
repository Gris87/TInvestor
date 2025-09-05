#pragma once



#include "src/threads/bidirtrading/ibidirtradingthreadfactory.h"



class BiDirTradingThreadFactory : public IBiDirTradingThreadFactory
{
public:
    BiDirTradingThreadFactory();
    ~BiDirTradingThreadFactory() override;

    BiDirTradingThreadFactory(const BiDirTradingThreadFactory& another)            = delete;
    BiDirTradingThreadFactory& operator=(const BiDirTradingThreadFactory& another) = delete;

    IBiDirTradingThread* newInstance(QObject* parent) const override;
};
