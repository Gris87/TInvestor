#pragma once



#include <QPixmap>



using Logos = QMap<QString, QPixmap*>; // Instrument UID => Logo



class ILogosDatabase
{
public:
    explicit ILogosDatabase() = default;
    virtual ~ILogosDatabase() = default;

    ILogosDatabase(const ILogosDatabase& another)            = delete;
    ILogosDatabase& operator=(const ILogosDatabase& another) = delete;

    virtual Logos readLogos()                                           = 0;
    virtual void  writeLogo(const QString& instrumentId, QPixmap* logo) = 0;
};
