#include "src/db/instruments/instrumentsdatabase.h"

#include <gtest/gtest.h>



class Test_InstrumentsDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        database = new InstrumentsDatabase();
    }

    void TearDown() override
    {
        delete database;
    }

    InstrumentsDatabase* database;
};



TEST_F(Test_InstrumentsDatabase, Test_constructor_and_destructor)
{
}
