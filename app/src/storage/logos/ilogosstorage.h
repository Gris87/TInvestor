#pragma once



#include <QMutex>
#include <QPixmap>



class ILogosStorage
{
public:
    ILogosStorage()          = default;
    virtual ~ILogosStorage() = default;

    ILogosStorage(const ILogosStorage& another)            = delete;
    ILogosStorage& operator=(const ILogosStorage& another) = delete;

    virtual void     lock()                                                    = 0;
    virtual void     unlock()                                                  = 0;
    virtual void     setLogo(const QString& instrumentId, const QPixmap& logo) = 0;
    virtual QPixmap* getLogo(const QString& instrumentId)                      = 0;
};
