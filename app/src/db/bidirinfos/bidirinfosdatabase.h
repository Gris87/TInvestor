#pragma once



#include "src/db/bidirinfos/ibidirinfosdatabase.h"

#include "src/utils/fs/dir/idirfactory.h"
#include "src/utils/fs/file/ifilefactory.h"



class BidirInfosDatabase : public IBidirInfosDatabase
{
public:
    explicit BidirInfosDatabase(IDirFactory* dirFactory, IFileFactory* fileFactory);
    ~BidirInfosDatabase() override;

    BidirInfosDatabase(const BidirInfosDatabase& another)            = delete;
    BidirInfosDatabase& operator=(const BidirInfosDatabase& another) = delete;

    BidirInfos readBidirInfos() override;

private:
    IFileFactory* mFileFactory;
};
