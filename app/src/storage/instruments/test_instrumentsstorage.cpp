#include "src/storage/instruments/instrumentsstorage.h"

#include <gtest/gtest.h>

#include "src/db/instruments/iinstrumentsdatabase_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::Return;
using ::testing::StrictMock;



class Test_InstrumentsStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentsDatabaseMock = new StrictMock<InstrumentsDatabaseMock>();

        storage = new InstrumentsStorage(instrumentsDatabaseMock);
    }

    void TearDown() override
    {
        delete storage;
        delete instrumentsDatabaseMock;
    }

    InstrumentsStorage*                  storage;
    StrictMock<InstrumentsDatabaseMock>* instrumentsDatabaseMock;
};



TEST_F(Test_InstrumentsStorage, Test_constructor_and_destructor)
{
}

TEST_F(Test_InstrumentsStorage, Test_readFromDatabase_and_getInstruments)
{
    const InSequence seq;

    Instruments instruments = storage->getInstruments();
    ASSERT_EQ(instruments.size(), 0);

    Instrument instrument1;
    Instrument instrument2;
    Instrument instrument3;

    instrument1.ticker         = "BANA";
    instrument1.name           = "Banana";
    instrument1.pricePrecision = 2;

    instrument2.ticker         = "BODO";
    instrument2.name           = "BODO NE BODO";
    instrument2.pricePrecision = 3;

    instrument3.ticker         = "ZARA";
    instrument3.name           = "Zaraza";
    instrument3.pricePrecision = 4;

    instruments["aaaaa"] = instrument1;
    instruments["bbbbb"] = instrument2;
    instruments["ccccc"] = instrument3;

    EXPECT_CALL(*instrumentsDatabaseMock, readInstruments()).WillOnce(Return(instruments));

    storage->readFromDatabase();
    instruments = storage->getInstruments();

    // clang-format off
    ASSERT_EQ(instruments.size(),                  3);
    ASSERT_EQ(instruments["aaaaa"].ticker,         "BANA");
    ASSERT_EQ(instruments["aaaaa"].name,           "Banana");
    ASSERT_EQ(instruments["aaaaa"].pricePrecision, 2);
    ASSERT_EQ(instruments["bbbbb"].ticker,         "BODO");
    ASSERT_EQ(instruments["bbbbb"].name,           "BODO NE BODO");
    ASSERT_EQ(instruments["bbbbb"].pricePrecision, 3);
    ASSERT_EQ(instruments["ccccc"].ticker,         "ZARA");
    ASSERT_EQ(instruments["ccccc"].name,           "Zaraza");
    ASSERT_EQ(instruments["ccccc"].pricePrecision, 4);
    // clang-format on
}

TEST_F(Test_InstrumentsStorage, Test_getMutex)
{
    ASSERT_NE(storage->getMutex(), nullptr);
}

TEST_F(Test_InstrumentsStorage, Test_mergeInstruments)
{
    const InSequence seq;

    Instruments instruments = storage->getInstruments();
    ASSERT_EQ(instruments.size(), 0);

    Instrument instrument1;
    Instrument instrument2;
    Instrument instrument3;

    instrument1.ticker         = "BANA";
    instrument1.name           = "Banana";
    instrument1.pricePrecision = 2;

    instrument2.ticker         = "BODO";
    instrument2.name           = "BODO NE BODO";
    instrument2.pricePrecision = 3;

    instrument3.ticker         = "ZARA";
    instrument3.name           = "Zaraza";
    instrument3.pricePrecision = 4;

    instruments["aaaaa"] = instrument1;
    instruments["bbbbb"] = instrument2;
    instruments["ccccc"] = instrument3;

    EXPECT_CALL(*instrumentsDatabaseMock, readInstruments()).WillOnce(Return(instruments));

    storage->readFromDatabase();
    instruments = storage->getInstruments();

    // clang-format off
    ASSERT_EQ(instruments.size(),                  3);
    ASSERT_EQ(instruments["aaaaa"].ticker,         "BANA");
    ASSERT_EQ(instruments["aaaaa"].name,           "Banana");
    ASSERT_EQ(instruments["aaaaa"].pricePrecision, 2);
    ASSERT_EQ(instruments["bbbbb"].ticker,         "BODO");
    ASSERT_EQ(instruments["bbbbb"].name,           "BODO NE BODO");
    ASSERT_EQ(instruments["bbbbb"].pricePrecision, 3);
    ASSERT_EQ(instruments["ccccc"].ticker,         "ZARA");
    ASSERT_EQ(instruments["ccccc"].name,           "Zaraza");
    ASSERT_EQ(instruments["ccccc"].pricePrecision, 4);
    // clang-format on

    Instruments newInstruments;

    Instrument newInstrument1;
    Instrument newInstrument2;

    newInstrument1.ticker         = "WADA";
    newInstrument1.name           = "Awada kedabra";
    newInstrument1.pricePrecision = 5;

    newInstrument2.ticker         = "NIGA";
    newInstrument2.name           = "Read my kniga";
    newInstrument2.pricePrecision = 6;

    newInstruments["bbbbb"] = newInstrument1;
    newInstruments["ddddd"] = newInstrument2;

    EXPECT_CALL(*instrumentsDatabaseMock, writeInstruments(Ne(Instruments())));

    storage->mergeInstruments(newInstruments);
    storage->mergeInstruments(newInstruments);

    instruments = storage->getInstruments();

    // clang-format off
    ASSERT_EQ(instruments.size(),                  4);
    ASSERT_EQ(instruments["aaaaa"].ticker,         "BANA");
    ASSERT_EQ(instruments["aaaaa"].name,           "Banana");
    ASSERT_EQ(instruments["aaaaa"].pricePrecision, 2);
    ASSERT_EQ(instruments["bbbbb"].ticker,         "WADA");
    ASSERT_EQ(instruments["bbbbb"].name,           "Awada kedabra");
    ASSERT_EQ(instruments["bbbbb"].pricePrecision, 5);
    ASSERT_EQ(instruments["ccccc"].ticker,         "ZARA");
    ASSERT_EQ(instruments["ccccc"].name,           "Zaraza");
    ASSERT_EQ(instruments["ccccc"].pricePrecision, 4);
    ASSERT_EQ(instruments["ddddd"].ticker,         "NIGA");
    ASSERT_EQ(instruments["ddddd"].name,           "Read my kniga");
    ASSERT_EQ(instruments["ddddd"].pricePrecision, 6);
    // clang-format on
}
