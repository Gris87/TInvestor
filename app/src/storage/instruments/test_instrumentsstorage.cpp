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
