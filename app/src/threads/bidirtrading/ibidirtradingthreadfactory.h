#pragma once



#include "src/threads/bidirtrading/ibidirtradingthread.h"



class IBiDirTradingThreadFactory
{
public:
    IBiDirTradingThreadFactory()          = default;
    virtual ~IBiDirTradingThreadFactory() = default;

    IBiDirTradingThreadFactory(const IBiDirTradingThreadFactory& another)            = delete;
    IBiDirTradingThreadFactory& operator=(const IBiDirTradingThreadFactory& another) = delete;

    virtual IBiDirTradingThread* newInstance(QObject* parent) const = 0;
};
