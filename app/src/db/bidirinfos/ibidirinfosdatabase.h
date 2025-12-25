#pragma once



#include "src/domain/bidirinfo/bidirinfo.h"



class IBidirInfosDatabase
{
public:
    explicit IBidirInfosDatabase() = default;
    virtual ~IBidirInfosDatabase() = default;

    IBidirInfosDatabase(const IBidirInfosDatabase& another)            = delete;
    IBidirInfosDatabase& operator=(const IBidirInfosDatabase& another) = delete;

    virtual BidirInfos readBidirInfos() = 0;
};
