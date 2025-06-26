#pragma once



#include "src/domain/logo/logo.h"



class ILogosStorage
{
public:
    ILogosStorage()          = default;
    virtual ~ILogosStorage() = default;

    ILogosStorage(const ILogosStorage& another)            = delete;
    ILogosStorage& operator=(const ILogosStorage& another) = delete;

    virtual void  readFromDatabase()                                        = 0;
    virtual void  readLock()                                                = 0;
    virtual void  readUnlock()                                              = 0;
    virtual void  writeLock()                                               = 0;
    virtual void  writeUnlock()                                             = 0;
    virtual void  setLogo(const QString& instrumentId, const QPixmap& logo) = 0;
    virtual Logo* getLogo(const QString& instrumentId)                      = 0;
};
