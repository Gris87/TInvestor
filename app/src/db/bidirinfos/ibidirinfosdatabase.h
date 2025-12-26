#pragma once



#include "src/domain/bidirinfo/bidirinfo.h"
#include "src/utils/fs/file/ifile.h"



class IBidirInfosDatabase
{
public:
    explicit IBidirInfosDatabase() = default;
    virtual ~IBidirInfosDatabase() = default;

    IBidirInfosDatabase(const IBidirInfosDatabase& another)            = delete;
    IBidirInfosDatabase& operator=(const IBidirInfosDatabase& another) = delete;

    virtual BidirInfos readBidirInfos(std::shared_ptr<IFile> bidirInfoFile) = 0;
};
