#pragma once



#include <QMap>
#include <QPixmap>

#include "src/domain/logo/logo.h"



class ILogosDatabase
{
public:
    explicit ILogosDatabase() = default;
    virtual ~ILogosDatabase() = default;

    ILogosDatabase(const ILogosDatabase& another)            = delete;
    ILogosDatabase& operator=(const ILogosDatabase& another) = delete;

    virtual Logos prepareLogos()                                        = 0;
    virtual void  readLogo(const QString& instrumentId, QPixmap* logo)  = 0;
    virtual void  writeLogo(const QString& instrumentId, QPixmap* logo) = 0;
};
