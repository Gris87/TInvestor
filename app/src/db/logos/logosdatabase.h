#pragma once



#include "src/db/logos/ilogosdatabase.h"

#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class LogosDatabase : public ILogosDatabase
{
public:
    explicit LogosDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory);
    ~LogosDatabase() override;

    LogosDatabase(const LogosDatabase& another)            = delete;
    LogosDatabase& operator=(const LogosDatabase& another) = delete;

    Logos readLogos() override;
    void  writeLogo(const QString& instrumentId, QPixmap* logo) override;

private:
    IDirFactory*  mDirFactory;
    IFileFactory* mFileFactory;
};
