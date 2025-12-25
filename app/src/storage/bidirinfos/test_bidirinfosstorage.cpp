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

    bidirInfo1.ticker            = "BANA";
    bidirInfo1.name              = "Banana";
    bidirInfo1.lot               = 1;
    bidirInfo1.pricePrecision    = 2;
    bidirInfo1.minPriceIncrement = Quotation(0, 100000000);

    bidirInfo2.ticker            = "BODO";
    bidirInfo2.name              = "BODO NE BODO";
    bidirInfo2.lot               = 5;
    bidirInfo2.pricePrecision    = 3;
    bidirInfo2.minPriceIncrement = Quotation(0, 1000000);

    bidirInfo3.ticker            = "ZARA";
    bidirInfo3.name              = "Zaraza";
    bidirInfo3.lot               = 10;
    bidirInfo3.pricePrecision    = 4;
    bidirInfo3.minPriceIncrement = Quotation(0, 500000);

    bidirInfos["aaaaa"] = bidirInfo1;
    bidirInfos["bbbbb"] = bidirInfo2;
    bidirInfos["ccccc"] = bidirInfo3;

    EXPECT_CALL(*bidirInfosDatabaseMock, readBidirInfos()).WillOnce(Return(bidirInfos));

    storage->readFromDatabase();
    bidirInfos = storage->getBidirInfos();

    // clang-format off
    ASSERT_EQ(bidirInfos.size(),                     3);
    ASSERT_EQ(bidirInfos["aaaaa"].ticker,            "BANA");
    ASSERT_EQ(bidirInfos["aaaaa"].name,              "Banana");
    ASSERT_EQ(bidirInfos["aaaaa"].lot,               1);
    ASSERT_EQ(bidirInfos["aaaaa"].pricePrecision,    2);
    ASSERT_EQ(bidirInfos["aaaaa"].minPriceIncrement, Quotation(0, 100000000));
    ASSERT_EQ(bidirInfos["bbbbb"].ticker,            "BODO");
    ASSERT_EQ(bidirInfos["bbbbb"].name,              "BODO NE BODO");
    ASSERT_EQ(bidirInfos["bbbbb"].lot,               5);
    ASSERT_EQ(bidirInfos["bbbbb"].pricePrecision,    3);
    ASSERT_EQ(bidirInfos["bbbbb"].minPriceIncrement, Quotation(0, 1000000));
    ASSERT_EQ(bidirInfos["ccccc"].ticker,            "ZARA");
    ASSERT_EQ(bidirInfos["ccccc"].name,              "Zaraza");
    ASSERT_EQ(bidirInfos["ccccc"].lot,               10);
    ASSERT_EQ(bidirInfos["ccccc"].pricePrecision,    4);
    ASSERT_EQ(bidirInfos["ccccc"].minPriceIncrement, Quotation(0, 500000));
    // clang-format on
}

TEST_F(Test_BidirInfosStorage, Test_lock_and_unlock)
{
    storage->writeLock();
    storage->writeUnlock();
    storage->readLock();
    storage->readUnlock();
}
