#pragma once



#include "src/db/bidirinfos/ibidirinfosdatabase.h"

#include <gmock/gmock.h>



class BidirInfosDatabaseMock : public IBidirInfosDatabase
{
public:
    explicit BidirInfosDatabaseMock() :
        IBidirInfosDatabase()
    {
    }
    ~BidirInfosDatabaseMock() override = default;

    BidirInfosDatabaseMock(const BidirInfosDatabaseMock& another)            = delete;
    BidirInfosDatabaseMock& operator=(const BidirInfosDatabaseMock& another) = delete;

    MOCK_METHOD(BidirInfos, readBidirInfos, (), (override));
};
