#pragma once



#include "src/storage/logos/ilogosstorage.h"

#include <QReadWriteLock>

#include "src/db/logos/ilogosdatabase.h"



class LogosStorage : public ILogosStorage
{
public:
    explicit LogosStorage(ILogosDatabase* logosDatabase);
    ~LogosStorage() override;

    LogosStorage(const LogosStorage& another)            = delete;
    LogosStorage& operator=(const LogosStorage& another) = delete;

    void  readFromDatabase() override;
    void  readLock() override;
    void  readUnlock() override;
    void  writeLock() override;
    void  writeUnlock() override;
    void  setLogo(const QString& instrumentId, const QPixmap& logo) override;
    Logo* getLogo(const QString& instrumentId) override;

private:
    ILogosDatabase* mLogosDatabase;
    QReadWriteLock* mRwMutex;
    Logos           mLogos;
    Logo            mNotFoundLogo;
};
