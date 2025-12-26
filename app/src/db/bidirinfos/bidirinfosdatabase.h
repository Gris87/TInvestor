#pragma once



#include "src/db/bidirinfos/ibidirinfosdatabase.h"

#include "src/utils/fs/dir/idirfactory.h"



class BidirInfosDatabase : public IBidirInfosDatabase
{
public:
    explicit BidirInfosDatabase(IDirFactory* dirFactory);
    ~BidirInfosDatabase() override;

    BidirInfosDatabase(const BidirInfosDatabase& another)            = delete;
    BidirInfosDatabase& operator=(const BidirInfosDatabase& another) = delete;

    BidirInfos readBidirInfos(std::shared_ptr<IFile> bidirInfoFile) override;
};
