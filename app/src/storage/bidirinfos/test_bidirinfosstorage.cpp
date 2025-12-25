#include "src/storage/bidirinfos/bidirinfosstorage.h"

#include <gtest/gtest.h>

#include "src/db/bidirinfos/ibidirinfosdatabase_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BidirInfosStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        bidirInfosDatabaseMock = new StrictMock<BidirInfosDatabaseMock>();

        storage = new BidirInfosStorage(bidirInfosDatabaseMock);
    }

    void TearDown() override
    {
        delete storage;
        delete bidirInfosDatabaseMock;
    }

    BidirInfosStorage*                  storage;
    StrictMock<BidirInfosDatabaseMock>* bidirInfosDatabaseMock;
};



TEST_F(Test_BidirInfosStorage, Test_constructor_and_destructor)
{
}

TEST_F(Test_BidirInfosStorage, Test_readFromDatabase_and_getBidirInfos)
{
    const InSequence seq;

    BidirInfos bidirInfos = storage->getBidirInfos();
    ASSERT_EQ(bidirInfos.size(), 0);

    BidirInfo bidirInfo1;
    BidirInfo bidirInfo2;
    BidirInfo bidirInfo3;

    bidirInfo1.spread     = 0.1f;
    bidirInfo1.minYield   = 1.1f;
    bidirInfo1.totalYield = 2.1f;

    bidirInfo2.spread     = 0.2f;
    bidirInfo2.minYield   = 1.2f;
    bidirInfo2.totalYield = 2.2f;

    bidirInfo3.spread     = 0.3f;
    bidirInfo3.minYield   = 1.3f;
    bidirInfo3.totalYield = 2.3f;

    bidirInfos["aaaaa"] = bidirInfo1;
    bidirInfos["bbbbb"] = bidirInfo2;
    bidirInfos["ccccc"] = bidirInfo3;

    EXPECT_CALL(*bidirInfosDatabaseMock, readBidirInfos()).WillOnce(Return(bidirInfos));

    storage->readFromDatabase();
    bidirInfos = storage->getBidirInfos();

    // clang-format off
    ASSERT_EQ(bidirInfos.size(),                3);
    ASSERT_NEAR(bidirInfos["aaaaa"].spread,     0.1f, 0.0001f);
    ASSERT_NEAR(bidirInfos["aaaaa"].minYield,   1.1f, 0.0001f);
    ASSERT_NEAR(bidirInfos["aaaaa"].totalYield, 2.1f, 0.0001f);
    ASSERT_NEAR(bidirInfos["bbbbb"].spread,     0.2f, 0.0001f);
    ASSERT_NEAR(bidirInfos["bbbbb"].minYield,   1.2f, 0.0001f);
    ASSERT_NEAR(bidirInfos["bbbbb"].totalYield, 2.2f, 0.0001f);
    ASSERT_NEAR(bidirInfos["ccccc"].spread,     0.3f, 0.0001f);
    ASSERT_NEAR(bidirInfos["ccccc"].minYield,   1.3f, 0.0001f);
    ASSERT_NEAR(bidirInfos["ccccc"].totalYield, 2.3f, 0.0001f);
    // clang-format on
}

TEST_F(Test_BidirInfosStorage, Test_lock_and_unlock)
{
    storage->writeLock();
    storage->writeUnlock();
    storage->readLock();
    storage->readUnlock();
}
