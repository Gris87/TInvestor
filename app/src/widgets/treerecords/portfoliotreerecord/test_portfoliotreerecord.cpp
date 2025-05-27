#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecord.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"



using ::testing::StrictMock;



class Test_PortfolioTreeRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();

        record = new PortfolioTreeRecord(instrumentsStorageMock);
    }

    void TearDown() override
    {
        delete record;
        delete instrumentsStorageMock;
    }

    PortfolioTreeRecord*                record;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
};



TEST_F(Test_PortfolioTreeRecord, Test_constructor_and_destructor)
{
}
