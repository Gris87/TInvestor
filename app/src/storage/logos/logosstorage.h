#pragma once



#include "src/storage/logos/ilogosstorage.h"

#include <QMap>

#include "src/db/logos/ilogosdatabase.h"



class LogosStorage : public ILogosStorage
{
public:
    explicit LogosStorage(ILogosDatabase* logosDatabase);
    ~LogosStorage() override;

    LogosStorage(const LogosStorage& another)            = delete;
    LogosStorage& operator=(const LogosStorage& another) = delete;

    void     lock() override;
    void     unlock() override;
    void     setLogo(const QString& instrumentId, const QPixmap& logo) override;
    QPixmap* getLogo(const QString& instrumentId) override;

private:
    ILogosDatabase*         mLogosDatabase;
    QMutex*                 mMutex;
    QMap<QString, QPixmap*> mLogos; // Instrument UID => Logo
};
