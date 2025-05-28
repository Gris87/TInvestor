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

    QTreeWidget treeWidget;
    treeWidget.setColumnCount(PORTFOLIO_COLUMN_COUNT);

    CategoryTreeItem categoryTreeItem(&treeWidget, "Hello");

    const IPortfolioTreeRecord* record = factory.newInstance(&instrumentsStorageMock, &categoryTreeItem, "aaaaa", nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
