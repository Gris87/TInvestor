#include "src/storage/instruments/instrumentssstorage.h"

#include <gtest/gtest.h>



class Test_InstrumentsStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        storage = new InstrumentsStorage();
    }

    void TearDown() override
    {
        delete storage;
    }

    InstrumentsStorage* storage;
};



TEST_F(Test_InstrumentsStorage, Test_constructor_and_destructor)
{
}
