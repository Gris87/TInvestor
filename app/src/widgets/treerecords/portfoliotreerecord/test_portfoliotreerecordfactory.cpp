#include "src/widgets/treerecords/portfoliotreerecord/portfoliotreerecordfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"



using ::testing::StrictMock;



TEST(Test_PortfolioTreeRecordFactory, Test_constructor_and_destructor)
{
    const PortfolioTreeRecordFactory factory;
}

TEST(Test_PortfolioTreeRecordFactory, Test_newInstance)
{
    const PortfolioTreeRecordFactory factory;

    StrictMock<InstrumentsStorageMock> instrumentsStorageMock;

    const IPortfolioTreeRecord* record = factory.newInstance(&instrumentsStorageMock, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
